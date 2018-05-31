#ifndef KBUCKET_H
#define KBUCKET_H

#include "kstroage/kstroage.h"
#include "keystore/keystore.h"

typedef struct kbucket_t kbucket_t;

struct kbucket_t {
    kstroage_t static_info;
    kstroage_t dyn_info;
    uint16_t router[160*8];
    keystore_t *keystore;
};

void kbucket_init(ascl_t *ascl, kbucket_t *kbucket,keystore_t *keystore);

void kbucket_insert(kbucket_t *kbucket,
        keystore_nodeid_t nodeid,
        const uint8_t *static_info,
        const uint8_t *dyn_info);

void kbucket_delete(kbucket_t *kbucket,
        keystore_nodeid_t nodeid);

void kbucket_update(kbucket_t *kbucket,
        keystore_nodeid_t nodeid,
        const uint8_t *dyn_info);

void kbucket_get_static(kbucket_t *kbucket,
        keystore_nodeid_t nodeid
        uint8_t *info);

void kbucket_get_dyn(kbucket_t *kbucket,
        keystore_nodeid_t nodeid
        uint8_t *info);

#endif
