#ifndef KSTORAGE_H
#define KSTORAGE_H

#include "platform.h"
#include "ascl/ascl.h"
#include "ascl/file.h"

typedef struct kstorage_t kstorage_t;

struct kstorage_t {
    size_t item_size;
    size_t total;
    ascl_file_t *file;
}

// void kstorage_init(ascl_t *ascl,kstorage_t *ks);

int kstorage_insert(kstorage_t *ks, uint8_t *data);

int kstorage_update(kstorage_t *ks, int index, uint8_t *data);

int kstorage_get(kstorage_t *ks, int index, uint8_t *data);

#endif
