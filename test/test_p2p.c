#include "ascl/ascl.h"
#include "ascl/udp.h"

void send_cb(ascl_udp_t *udp,int status) {
    printf("send success\n");
}

void recv_cb (ascl_udp_t *udp,
        const ascl_sockaddr_t *addr,
        size_t nread,
        uint8_t *data) {
    if (nread > 0 ){
        printf("recv data from %d.%d.%d.%d:%d\n",
                addr->addr.ipv4[0],
                addr->addr.ipv4[1],
                addr->addr.ipv4[2],
                addr->addr.ipv4[3],
                addr->port);
        printf("recv data len %d\n",nread);
        ascl_sockaddr_t remote = {
            NET_TYPE_IPV4,
            8899,
            {
                0,0,0,0
            }
        };
        ascl_udp_send(udp,&remote,nread,data);
        printf("recv data %s\n",data);
    }
}

int main() {
    ascl_t *ascl = ascl_init();
    ascl_udp_t *client = ascl_udp_init(ascl);
    ascl_sockaddr_t sock = {
        NET_TYPE_IPV4,
        8888,
        {
            0,0,0,0
        }
    };
    ascl_udp_bind(client,&sock,ASCL_DEFAULT);
    ascl_udp_set_handle(client,send_cb,recv_cb);
    ascl_udp_recv(client);
    ascl_sockaddr_t remote = {
        NET_TYPE_IPV4,
        8899,
        {
            0,0,0,0
        }
    };
    char a[] = "hello world\n";
    ascl_udp_send(client,&remote,sizeof(a),a);
    ascl_run(ascl);
    return 0;
}
