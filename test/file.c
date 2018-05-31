#include "ascl/ascl.h"
#include "ascl/file.h"
#include "stdio.h"
#include "uv.h"

static uint8_t buffer[20];

static void read_cb(ascl_file_t *file,size_t nread,uint8_t *data) {
    printf("%s\n",data);
    printf("%d\n",nread);
    /* printf("%s\n",uv_err_name(nread)); */
    /* printf("%s\n",uv_strerror(nread)); */
}

static void write_cb(ascl_file_t *file,size_t status) {

}

static void init_cb(ascl_file_t *file,int status) {
    printf ("init success %d\n", status);
    ascl_file_write(file,-1,10,"12423532353");
    /* ascl_file_read(file,-1,10,buffer); */
}

int main() {
    ascl_t *ascl = ascl_init();

    ascl_file_t *file = ascl_file_init(ascl,"./test.txt",ASCL_FILE_WRONLY);
    ascl_file_set_handle(file,init_cb,write_cb,read_cb,NULL);
    
    ascl_run(ascl);
    /* ascl_close(ascl); */
    return 0;
}
