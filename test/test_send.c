#include "ascl/ascl.h"
#include "ascl/udp.h"

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
    ascl_udp_set_handle(client,NULL,NULL);
    ascl_sockaddr_t remote = {
        NET_TYPE_IPV4,
        8899,
        {
            0,0,0,0
        }
    };
    char a[] = "hello world";
    ascl_udp_send(client,&remote,sizeof(a),a);
    ascl_run(ascl);
    return 0;
}
