#ifndef ASCL_UDP_H
#define ASCL_UDP_H

#include "ascl.h"
#include "platform.h"

#define ASCL_REUSE_ADDR 4
#define ASCL_DEFAULT 0

typedef struct ascl_udp_t ascl_udp_t;

typedef enum NetAddrType_t {
    NET_TYPE_NONE, NET_TYPE_IPV4, NET_TYPE_IPV6
} NetAddrType_t;

typedef struct sockaddr_t {
    NetAddrType_t type;
    uint16_t port;
    union {
        uint8_t ipv4[4];
        uint8_t ipv6[16];
    } addr;
} ascl_sockaddr_t;

typedef void (*ascl_udp_recv_cb)(ascl_udp_t *udp,
        const ascl_sockaddr_t *addr,
        size_t nread,
        uint8_t *data);

typedef void (*ascl_udp_send_cb)(ascl_udp_t *udp,
        int status);

ascl_udp_t *ascl_udp_init(ascl_t *ascl);

int ascl_udp_bind(ascl_udp_t *udp,
        const ascl_sockaddr_t *addr,
        unsigned int flags);

void ascl_udp_set_handle(ascl_udp_t *udp,
        ascl_udp_send_cb send_cb,
        ascl_udp_recv_cb recv_cb);

int ascl_udp_send(ascl_udp_t *udp,
        const ascl_sockaddr_t *addr,
        size_t len,
        uint8_t *data);

int ascl_udp_recv(ascl_udp_t *udp);

#endif
