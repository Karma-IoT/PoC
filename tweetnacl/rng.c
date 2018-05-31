#include "tweetnacl.h"

static crypto_rng global_rng;

void set_rng(crypto_rng rng){
    global_rng = rng;
}

void randombytes(u8 *dest,u64 size){
    global_rng(dest,size);
}
