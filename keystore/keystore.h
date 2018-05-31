#ifndef KEYSTORE_H
#define KEYSTORE_H

#include "ascl/ascl.h"
#include "platform.h"
#include "tweetnacl/tweetnacl.h"

#define KEYSTORE_NODEID_LENGTH 20
#define KEYSTORE_SIGNATURE_LENGTH 64

typedef struct keystore_t keystore_t;

typedef uint8_t keystore_nodeid_t[KEYSTORE_NODEID_LENGTH];
typedef uint8_t keystore_prikey_t[crypto_sign_SECRETKEYBYTES];
typedef uint8_t keystore_pubkey_t[crypto_sign_PUBLICKEYBYTES];

typedef void (*keystore_init_cb)(keystore_t *keystore);

struct keystore_t {
    keystore_prikey_t prikey;
    keystore_pubkey_t pubkey;
    keystore_nodeid_t nodeid;
    keystore_init_cb init_cb;
};

void keystore_init(ascl_t *ascl,keystore_t *keystore);

void keystore_gen(ascl_t *ascl,keystore_t *keystore);

void keystore_set_handle(keystore_t *keystore,
        keystore_init_cb init_cb);

void keystore_sign(keystore_t *keystore,
        const uint8_t *m,
        const size_t mlen,
        uint8_t *sm,
        size_t *smlen);

int keystore_verify(keystore_t *keystore,
        uint8_t *m,
        size_t *mlen,
        uint8_t *sm,
        size_t smlen);

void keystore_close(keystore_t *keystore);

#endif
