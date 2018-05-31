#include "kbucket.h"

void kbucket_init(ascl_t *ascl, kbucket_t *kbucket,keystore_t *keystore) {
    kbucket->static_info.item_size = 54;
    kbucket->static_info.total = 160*8;
    kbucket->file = ascl_file_init(ascl,"static.dat",ASCL_FILE_WRONLY | ASCL_FILE_CREATE);
    kbucket->dyn_info.item_size = 100;
    kbucket->dyn_info.total = 160*8;
    kbucket->file = ascl_file_init(ascl,"dyn.dat",ASCL_FILE_WRONLY | ASCL_FILE_CREATE);
    kbucket->keystore = keystore;
    memset(kbucket->router,0,160*8);
}

static int nequal(keystore_nodeid_t n1,keystore_nodeid_t n2){
    return 0;
}

void kbucket_insert(kbucket_t *kbucket,
        keystore_nodeid_t nodeid,
        const uint8_t *static_info,
        const uint8_t *dyn_info) {
    int ibucket = nequal(kbucket->keystore->nodeid,nodeid);
    int i = ibucket * 8;
    for (;i != ibucket + 8 || kbucket->router[i] != 0; i++);
    *(int *)(static_info + 53) = kstroage_insert(kbucket->dyn_info,dyn_info);
    kbucket->router[i] = kstroage_insert(kbucket->static_info,static_info);
}

void kbucket_update(kbucket_t *kbucket,
        keystore_nodeid_t nodeid,
        const uint8_t *dyn_info){
    uint8_t static_info[54];
    kbucket_get_static(kbucket,nodeid,static_info);
    int index = *(int *)(static_info + 53);
    kstorage_update(kbucket->dyn_info,index,dyn_info);
}

void kbucket_get_static(kbucket_t *kbucket,
        keystore_nodeid_t nodeid
        uint8_t *info) {
    int ibucket = nequal(kbucket->keystore->nodeid,nodeid);
    int i = ibucket * 8;
    for (;i != ibucket + 8 || kbucket->router[i] != 0; i++){
        kstorage_get(kbucket->static_info,i,info);
        int n = nequal(nodeid,info);
        if (n == 160) {
            break;
        }
    }
}

void kbucket_get_dyn(kbucket_t *kbucket,
        keystore_nodeid_t nodeid
        uint8_t *info) {
    uint8_t static_info[54];
    kbucket_get_static(kbucket,nodeid,static_info);
    int index = *(int *)(static_info + 53);
    kstorage_get(kbucket->dyn_info,index,info);
}

