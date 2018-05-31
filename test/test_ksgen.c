#include "keystore/keystore.h"
#include "ascl/random.h"
#include "tweetnacl/tweetnacl.h"

void init_cb(keystore_t *ks) {
    printf("keystore init success\n");
}

int main() {
    ascl_t *ascl = ascl_init();
    set_rng(random_bytes);
    keystore_t keystore;
    keystore_gen(ascl,&keystore);    
    keystore_set_handle(&keystore,init_cb);
    for (int i = 0;i < KEYSTORE_NODEID_LENGTH;i++){
        printf("%02x",keystore.nodeid[i]);
    }
    printf("\n");

    ascl_run(ascl);
    return 0;
}
