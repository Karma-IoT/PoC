#ifndef ROUTER_ADDRS_H
#define ROUTER_ADDRS_H

#include "ascl/ascl.h"
#include "ascl/file.h"
// #include "platform.h"

#define ROUTER_ADDRS_MAX_LENTH 14*4096
#define ROUTER_ADDR_MAX_LENTH  44

#define ROUTER_ADDR_TYPE_NONE_LEN 0
#define ROUTER_ADDR_TYPE_IPv4_LEN 4 + 2
#define ROUTER_ADDR_TYPE_IPv6_LEN 16 + 2
#define ROUTER_ADDR_TYPE_MAC_LEN  6
#define ROUTER_ADDR_TYPE_UART_LEN 1

typedef enum router_addr_type_t{
    Router_Addr_Type_NONE = 0,
    Router_Addr_Type_IPv4,
    Router_Addr_Type_IPv6,
    Router_Addr_Type_MAC,
    Router_Addr_Type_Uart
} router_addr_type_t;

typedef struct router_addrs_t router_addrs_t;

typedef uint8_t router_addr_iterator_t[ROUTER_ADDR_MAX_LENTH];

typedef void (*router_addrs_init_cb)(router_addrs_t *addrs,int status);

typedef void (*router_addrs_put_cb)(router_addrs_t *addrs,size_t index);

typedef void (*router_addrs_get_cb)(router_addrs_t *addrs);

struct router_addrs_t {
    ascl_file_t *file;
    // size_t pos;
    uint8_t *pos;
    router_addr_iterator_t it;
    router_addrs_init_cb init_cb;
    router_addrs_get_cb get_cb;
    router_addrs_put_cb put_cb;
};

typedef struct router_addr_t {
    router_addr_type_t type;
    union {
        uint8_t ipv4[ROUTER_ADDR_TYPE_IPv4_LEN];
        uint8_t ipv6[ROUTER_ADDR_TYPE_IPv6_LEN];
        uint8_t mac[Router_Addr_Type_MAC];
        uint8_t uart[Router_Addr_Type_Uart];
    }addr;
}router_addr_t;

int router_addrs_init(ascl_t *ascl,
        router_addrs_t *addrs);

int router_addrs_handle(router_addrs_t *addrs,
        router_addrs_init_cb init_cb,
        router_addrs_get_cb get_cb,
        router_addrs_put_cb put_cb);

int router_addrs_put(router_addrs_t *addrs,
        int addr_index,
        router_addr_t *addr);

int router_addrs_get(router_addrs_t *addrs,
        size_t addr_index);

int router_addr_next(router_addrs_t *addrs,
        router_addr_t *addr);

#endif
