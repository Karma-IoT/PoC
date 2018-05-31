#ifndef ASCL_FILE_H
#define ASCL_FILE_H

#include "ascl.h"
#include "platform.h"

#define ASCL_FILE_RDONLY 0x00
#define ASCL_FILE_WRONLY 0x01
#define ASCL_FILE_CREATE 0x40
#define ASCL_FILE_RDWR 0x1a4

typedef struct ascl_file_t ascl_file_t;

typedef void (*ascl_file_read_cb)(ascl_file_t *file,
        size_t nread,
        uint8_t *data);

typedef void (*ascl_file_write_cb)(ascl_file_t *file,
        size_t nlen);

typedef void (*ascl_file_init_cb)(ascl_file_t *file,
        int status);

ascl_file_t *ascl_file_init(ascl_t *ascl,
        const char *filename,
        int mode);
        // ascl_file_init_cb init_cb);

void ascl_file_set_handle(ascl_file_t *file,
        ascl_file_init_cb init_cb,
        ascl_file_write_cb write_cb,
        ascl_file_read_cb read_cb,
        void *context);

void *ascl_file_get_context(const ascl_file_t *file);

int ascl_file_read(ascl_file_t *file,
        size_t offset,
        size_t nlen,
        uint8_t *data);

int ascl_file_write(ascl_file_t *file,
        size_t offset,
        size_t nlen,
        uint8_t *data);

void ascl_file_close(ascl_file_t *file);

#endif
