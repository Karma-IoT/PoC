#ifndef PEERS_H
#define PEERS_H

#include "keystore/keystore.h"
#include "ascl/ascl.h"

#define ROUTER_PEER_INFO_LENGTH 200

#define ROUTER_PEERS_CACHE_LENGTH 8

typedef uint8_t router_peer_info_t[ROUTER_PEER_INFO_LENGTH];

typedef struct router_peer_index_t {
    uint16_t index;
    keystore_nodeid_t nodeid;
} router_peer_index_t;

typedef void (*router_peers_init_cb)(router_peers_t *peers);

typedef void 

typedef struct router_peers_t {
    router_peer_index_t cache[ROUTER_PEERS_CACHE_LENGTH];
    router_peers_init_cb init_cb;
} router_peers_t;

void router_peers_init(ascl_t *ascl,router_peers_t *peers);

int router_peers_get_info(router_peers_t *peers,const int index);

int router_peers_insert_info(router_peers_t *peers);

typedef struct router_peers_iterator_t router_peers_iterator_t;

router_peers_iterator_t *router_peers_iterator_begin(router_peers_t *peers);

int router_peers_next(router_peers_t *peers,router_peers_iterator_t *it);

int router_peers_iterator_end(router_peers_iterator_t *it);

router_peers_close(router_peers_t *peers);

#endif
