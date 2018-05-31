#include "keystore/keystore.h"
#include "platform.h"
#include "rmd160/rmd160.h"
#include "ascl/ascl.h"
#include "ascl/file.h"

#define RMDsize 160

static void read_cb(ascl_file_t *file,size_t nread,uint8_t *data){
    keystore_t *keystore = ascl_file_get_context(file);
    ascl_file_close(file);
    if (keystore->init_cb) {
        (keystore->init_cb)(keystore);
    }
}

static void init_cb(ascl_file_t *file,int status){
    keystore_t *keystore = ascl_file_get_context(file);
    if(status != 0){
        /* karma_printf("") */
    }
    ascl_file_read(file,-1,
            crypto_sign_SECRETKEYBYTES + 
            crypto_sign_PUBLICKEYBYTES + 
            KEYSTORE_NODEID_LENGTH,
            (uint8_t *)keystore);
}

void keystore_init(ascl_t *ascl,keystore_t *keystore) {
    ascl_file_t *file = ascl_file_init(ascl,"keystore.dat",ASCL_FILE_RDONLY);
    ascl_file_set_handle(file,init_cb,NULL,read_cb,keystore);
}

static void rmd160_hash(uint8_t *msg,size_t mlen,uint8_t *hashcode){
    dword MDbuf[RMDsize/32];
    dword X[16];
    for (int k = mlen;k > 63;k -=64){
        for(int i = 0;i < 16;i ++){
            X[i] = BYTES_TO_DWORD(msg);
            msg += 4;
        }
        compress(MDbuf, X);
    }
    MDfinish(MDbuf, msg, mlen, 0);
    for (int i=0; i<RMDsize/8; i+=4) {
        hashcode[i]   =  MDbuf[i>>2];
        hashcode[i+1] = (MDbuf[i>>2] >>  8);
        hashcode[i+2] = (MDbuf[i>>2] >> 16);
        hashcode[i+3] = (MDbuf[i>>2] >> 24);
    }
}

static void gen_cb(ascl_file_t *file,int status){
    keystore_t *keystore = ascl_file_get_context(file);
    ascl_file_write(file,-1,
            crypto_sign_SECRETKEYBYTES + crypto_sign_PUBLICKEYBYTES + KEYSTORE_NODEID_LENGTH,
            (uint8_t *)keystore);
}

static void write_cb(ascl_file_t *file,size_t status) {
    keystore_t *keystore = ascl_file_get_context(file);
    ascl_file_close(file);
    if (keystore->init_cb) {
        (keystore->init_cb)(keystore);
    }
}

void keystore_gen(ascl_t *ascl,keystore_t *keystore){
    crypto_sign_keypair(keystore->pubkey,keystore->prikey);
    uint8_t hashcode[crypto_hash_BYTES];
    crypto_hash(hashcode,keystore->pubkey,crypto_sign_PUBLICKEYBYTES);
    rmd160_hash(hashcode,crypto_hash_BYTES,keystore->nodeid);
    ascl_file_t *file = ascl_file_init(ascl,"keystore.dat",ASCL_FILE_WRONLY | ASCL_FILE_CREATE);
    ascl_file_set_handle(file,gen_cb,write_cb,NULL,keystore);
}

void keystore_set_handle(keystore_t *keystore, keystore_init_cb init_cb) {
    keystore->init_cb = init_cb;
}

void keystore_sign(keystore_t *keystore,
        const uint8_t *m,
        const size_t mlen,
        uint8_t *sm,
        size_t *smlen){
    crypto_sign(sm,smlen,m,mlen,keystore->prikey);
}

int keystore_verify(keystore_t *keystore,
        uint8_t *m,
        size_t *mlen,
        uint8_t *sm,
        size_t smlen){
    return crypto_sign_open(m,mlen,sm,smlen,keystore->pubkey);
}

void keystore_close(keystore_t *keystore){
    karma_free(keystore);
}
