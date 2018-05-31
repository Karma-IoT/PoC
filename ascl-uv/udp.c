#include "ascl/udp.h"
#include "uv.h"
#include "platform.h"

#define ascl_offsetof(type,member) \
    ((size_t)(&((type*)0)->member))

#define ascl_to_struct(ptr,type,member) \
    ((type*)((char*)(ptr)-ascl_offsetof(type,member)))

struct ascl_udp_t {
    uv_udp_t udp;
    uv_udp_send_t sender;
    ascl_udp_send_cb send_cb;
    ascl_udp_recv_cb recv_cb;
};

ascl_udp_t *ascl_udp_init(ascl_t *ascl) {
    ascl_udp_t *udp = (ascl_udp_t *)karma_malloc(sizeof(ascl_udp_t));
    int result = uv_udp_init((uv_loop_t *)ascl,&udp->udp);
    if (result == 0){
        return udp;
    } else {
        karma_free(udp);
        return NULL;
    }
}

static const struct sockaddr *convert_sockaddr(struct sockaddr *dst,
        const ascl_sockaddr_t *src) {
    if (src->type == NET_TYPE_IPV4) {
        struct sockaddr_in *ptr = (struct sockaddr_in *)dst;
        ptr->sin_family = AF_INET;
        char *dstport = (char *)&ptr->sin_port;
        char *srcport = (char *)&src->port;
        dstport[0] = srcport[1];
        dstport[1] = srcport[0];
        karma_memcpy(&ptr->sin_addr,&src->addr,4);
    } else if (src->type == NET_TYPE_IPV6) {
        struct sockaddr_in6 *ptr = (struct sockaddr_in6 *)dst;
        ptr->sin6_family = AF_INET6;
        char *dstport = (char *)&ptr->sin6_port;
        char *srcport = (char *)&src->port;
        dstport[0] = srcport[1];
        dstport[1] = srcport[0];
        karma_memcpy(&ptr->sin6_addr,&src->addr,16);
    }
    return (const struct sockaddr *)dst;
}

static const ascl_sockaddr_t *convert_ascl_sockaddr(ascl_sockaddr_t *dst,
        const struct sockaddr *src) {
    if (src->sa_family == AF_INET) {
        struct sockaddr_in *psrc = (struct sockaddr_in *)src;
        char *dstport = (char *)&dst->port;
        char *srcport = (char *)&psrc->sin_port;
        dstport[0] = srcport[1];
        dstport[1] = srcport[0];
        karma_memcpy(&dst->addr,&psrc->sin_addr,4);
    } else if (src->sa_family == AF_INET6) {
        struct sockaddr_in6 *psrc = (struct sockaddr_in6 *)src;
        char *dstport = (char *)&dst->port;
        char *srcport = (char *)&psrc->sin6_port;
        dstport[0] = srcport[1];
        dstport[1] = srcport[0];
        karma_memcpy(&dst->addr,&psrc->sin6_addr,16);
    }
    return (const ascl_sockaddr_t *)dst;
}

int ascl_udp_bind(ascl_udp_t *udp,
        const ascl_sockaddr_t* addr,
        unsigned int flags) {
    struct sockaddr sock;
    uv_udp_bind(&udp->udp,
            convert_sockaddr(&sock,addr),
            flags);
}

void ascl_udp_set_handle(ascl_udp_t *udp,
        ascl_udp_send_cb send_cb,
        ascl_udp_recv_cb recv_cb) {
    udp->send_cb = send_cb;
    udp->recv_cb = recv_cb;
}

static void on_send(uv_udp_send_t* req, int status) {
    ascl_udp_t *udp = ascl_to_struct(req,ascl_udp_t,sender);
    if (udp->send_cb) {
        (udp->send_cb)(udp,status);
    }
}

int ascl_udp_send(ascl_udp_t *udp,
        const ascl_sockaddr_t *addr,
        size_t len,
        uint8_t *data) {
    struct sockaddr sock;
    uv_buf_t buf = uv_buf_init(data,len);
    uv_udp_send(&udp->sender,&udp->udp,&buf,1,convert_sockaddr(&sock,addr),on_send);
}

static void alloc_buffer(uv_handle_t* handle, size_t suggested_size, uv_buf_t* buf) {
  buf->base = karma_malloc(suggested_size);
  buf->len = suggested_size;
}

static void on_recv(uv_udp_t* handle, ssize_t nread, const uv_buf_t* buf, const struct sockaddr* addr, unsigned flags) {
    ascl_udp_t *udp = ascl_to_struct(handle,ascl_udp_t,udp);
    ascl_sockaddr_t saddr;
    if (udp->recv_cb && addr) {
        udp->recv_cb(udp,convert_ascl_sockaddr(&saddr,addr),nread,buf->base);
    }
}

int ascl_udp_recv(ascl_udp_t *udp) {
    return uv_udp_recv_start(&udp->udp,alloc_buffer,on_recv);
}
