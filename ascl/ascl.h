#ifndef ASCL_H
#define ASCL_H

typedef struct ascl_t ascl_t;

ascl_t *ascl_init();

int ascl_run(ascl_t *ascl);

void ascl_close(ascl_t *ascl);

#endif
