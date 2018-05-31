#include "router/addrs.h"

static uint8_t addr_type_index[] = {
    ROUTER_ADDR_TYPE_NONE_LEN,
    ROUTER_ADDR_TYPE_IPv4_LEN,
    ROUTER_ADDR_TYPE_IPv6_LEN,
    ROUTER_ADDR_TYPE_MAC_LEN,
    ROUTER_ADDR_TYPE_UART_LEN
};

static void init_cb (ascl_file_t *file,int status) {
    router_addrs_t *addrs = (router_addrs_t *)ascl_file_get_context(file);
    if (addrs->init_cb) {
        addrs->init_cb(addrs,status);
    }
}

static void write_cb(ascl_file_t *file,size_t nlen) {
    router_addrs_t *addrs = (router_addrs_t *)ascl_file_get_context(file);
    if (addrs->put_cb) {
        addrs->put_cb(addrs,nlen);
    }
}

struct s_arg {
    router_addr_t *addr;
    int addr_index;
};

static void read_cb(ascl_file_t *file,size_t nread,uint8_t *data) { 
    router_addrs_t *addrs = (router_addrs_t *)ascl_file_get_context(file);
    /* size_t pos = index * ROUTER_ADDR_MAX_LENTH; */
    if (addrs->pos) {
        struct s_arg *arg = (struct s_arg*)addrs->pos;
        router_addr_t *addr = arg->addr;
        size_t pos = arg->addr_index * ROUTER_ADDR_MAX_LENTH;
        uint8_t *pointer = addrs->it + addrs->it[0];
        *pointer = addr->type;
        karma_memcpy(pointer + 1,&addr->addr,addr_type_index[*pointer]);
        ascl_file_write(file,pos,ROUTER_ADDR_MAX_LENTH,addrs->it);
    } else {
        if (addrs->get_cb) {
            addrs->get_cb(addrs);
        }
    }
}

int router_addrs_init(ascl_t *ascl,
        router_addrs_t *addrs) {
    addrs->file = ascl_file_init(ascl,"addrs.dat",ASCL_FILE_RDWR | ASCL_FILE_CREATE);
    ascl_file_set_handle(addrs->file,init_cb,write_cb,read_cb,addrs);
}

int router_addrs_handle(router_addrs_t *addrs,
        router_addrs_init_cb init_cb,
        router_addrs_get_cb get_cb,
        router_addrs_put_cb put_cb) {
    addrs->init_cb = init_cb;
    addrs->get_cb = get_cb;
    addrs->put_cb = put_cb;
}

int router_addrs_put(router_addrs_t *addrs,
        int addr_index,
        router_addr_t *addr) {
    size_t pos = addr_index * ROUTER_ADDR_MAX_LENTH;
    struct s_arg arg;
    arg.addr = addr;
    arg.addr_index = addr_index;
    addrs->pos = (uint8_t *)&arg;
    return ascl_file_read(addrs->file,pos,ROUTER_ADDR_MAX_LENTH,addrs->it);
}

int router_addrs_get(router_addrs_t *addrs,
        size_t index) {
    size_t pos = index * ROUTER_ADDR_MAX_LENTH;
    addrs->pos = NULL;
    return ascl_file_read(addrs->file,pos,ROUTER_ADDR_MAX_LENTH,addrs->it);
}

int router_addr_next(router_addrs_t *addrs,
        router_addr_t *addr) {
    addrs->pos = addrs->it;
    uint8_t len = addrs->it[0];
    addr->type = *(++ addrs->pos);
    karma_memcpy(&addr->addr,addrs->pos + 1,addr_type_index[*addrs->pos]);
    addrs->pos += addr_type_index[*addrs->pos];
    return len - (addrs->pos - addrs->it);
}

