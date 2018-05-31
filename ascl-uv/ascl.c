#include "ascl/ascl.h"
#include "uv.h"
#include "platform.h"

#define ASCL_IMPL_LIBUV

struct ascl_t {
    uv_loop_t loop;
};

ascl_t *ascl_init() {
    ascl_t *ascl = (ascl_t*)karma_malloc(sizeof(ascl_t));
    uv_loop_init(&ascl->loop);
    return ascl;
}

int ascl_run(ascl_t *ascl) {
    uv_run(&ascl->loop, UV_RUN_DEFAULT);
}

void ascl_close(ascl_t *ascl) {
    uv_loop_close(&ascl->loop);
    karma_free(ascl);
}
