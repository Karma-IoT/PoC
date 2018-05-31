#include "ascl/random.h"
#include "stdlib.h"
#include "time.h"

int random_int() {
    srand(time(NULL));
    return rand();
}

int random_bytes(uint8_t *buf,size_t len) {
    srand(time(NULL));
    for (int i = 0;i < len;i++) {
        buf[i] = rand();
    }
}

