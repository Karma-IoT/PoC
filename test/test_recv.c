#include "ascl/ascl.h"
#include "ascl/udp.h"

void recv_cb (ascl_udp_t *udp,
        const ascl_sockaddr_t *addr,
        size_t nread,
        uint8_t *data) {
    if (nread > 0 ){
        printf("recv data len %d\n",nread);
        printf("recv data %s\n",data);
        printf("recv data from %d.%d.%d.%d:%d\n",
                addr->port,
                addr->addr.ipv4[0],
                addr->addr.ipv4[1],
                addr->addr.ipv4[2],
                addr->addr.ipv4[3]);
    }
}

int main() {
    ascl_t *ascl = ascl_init();

    ascl_udp_t *server = ascl_udp_init(ascl);
    ascl_sockaddr_t sock = {
        NET_TYPE_IPV4,
        8888,
        {
            0,0,0,0
        }
    };
    ascl_udp_bind(server,&sock,ASCL_DEFAULT);
    ascl_udp_set_handle(server,NULL,recv_cb);
    ascl_udp_recv(server);

    ascl_run(ascl);
    return 0;
}

