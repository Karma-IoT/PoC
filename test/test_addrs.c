#include "router/addrs.h"
#include "stdio.h"

int main() {
    router_addrs_t addrs;
    ascl_t *ascl = ascl_init();
    router_addrs_init(ascl,&addrs);
    /* router_addrs_handle() */
    return 0;
}
