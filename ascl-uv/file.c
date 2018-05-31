#include "ascl/file.h"
#include "uv.h"

struct ascl_file_t {
    uv_fs_t file;
    ascl_t *ascl;
    ascl_file_init_cb init_cb;
    ascl_file_write_cb write_cb;
    ascl_file_read_cb read_cb;
    uint8_t *data;
    size_t nlen;
    int fd;
    /* void *context; */
};

static void on_init(uv_fs_t *req) {
    ascl_file_t *file = (ascl_file_t*)req;
    int status = req->result;
    file->fd = req->result;
    uv_fs_req_cleanup(req);
    if (file->init_cb) {
        file->init_cb(file,status);
    }
}

ascl_file_t *ascl_file_init(ascl_t *ascl,
        const char *filename,
        int mode){
        /* ascl_file_init_cb init_cb) { */
    ascl_file_t *file = (ascl_file_t *)karma_malloc(sizeof(ascl_file_t));
    file->ascl = ascl;
    /* file->init_cb = init_cb; */
    uv_fs_open((uv_loop_t *)ascl,&file->file,filename,mode,0x1a4,on_init);
    return file;
}

void ascl_file_set_handle(ascl_file_t *file,
        ascl_file_init_cb init_cb,
        ascl_file_write_cb write_cb,
        ascl_file_read_cb read_cb,
        void *context) {
    file->write_cb = write_cb;
    file->read_cb = read_cb;
    file->init_cb = init_cb;
    uv_req_set_data((uv_req_t*)file,context);
}

void *ascl_file_get_context(const ascl_file_t *file) {
    return uv_req_get_data((const uv_req_t*)file);
}

static void on_read(uv_fs_t *req) {
    ascl_file_t *file = (ascl_file_t *)req;
    size_t len = req->result;
    uv_fs_req_cleanup(req);
    if(file->read_cb) {
        (file->read_cb)(file,len,file->data);
    }
}

int ascl_file_read(ascl_file_t *file,
        size_t offset,
        size_t nlen,
        uint8_t *data) {
    uv_buf_t iov = uv_buf_init(data, nlen);
    file->nlen = nlen;
    file->data = data;
    return uv_fs_read((uv_loop_t *)file->ascl,&file->file,file->fd,&iov,1,offset,on_read);
}

static void on_write(uv_fs_t *req) {
    ascl_file_t *file = (ascl_file_t *)req;
    size_t len = req->result;
    uv_fs_req_cleanup(req);
    if (file->write_cb) {
        (file->write_cb)(file,len);
    }
}

int ascl_file_write(ascl_file_t *file,
        size_t offset,
        size_t nlen,
        uint8_t *data) {
    uv_buf_t iov = uv_buf_init(data, nlen);
    file->nlen = nlen;
    printf("%d\n",nlen);
    /* file->data = data; */
    return uv_fs_write((uv_loop_t *)file->ascl,&file->file,file->fd,&iov,1,offset,on_write);
}

void ascl_file_close(ascl_file_t *file) {
    /* uv_fs_t close; */
    uv_fs_close((uv_loop_t *)file->ascl,&file->file,file->fd,NULL);
    /* karma_free(&file->file); */
    karma_free(file);
}
