#ifndef PFS_H
#define PFS_H

#include "keystore/keystore.h"
#include "router/router.h"

#define BOX_KEY_TTL 3600

typedef struct pfs_t pfs_t;

void pfs_init(pfs_t *pfs,keystore_t *keystore,router_t *router);

void pfs_run(pfs_t *pfs);

void pfs_box(pfs_t *pfs,keystore_nodeid_t remote,uint8_t *megbuf,size_t msglen,uint8_t *cipbuf,size_t ciplen);

void pfs_box_open(pfs_t *pfs,keystore_nodeid_t remote,uint8_t *megbuf,size_t msglen,uint8_t *cipbuf,size_t ciplen);

#endif
