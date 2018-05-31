#ifndef ROUTER_H
#define ROUTER_H

#include "keystore/keystore.h"

// #define ROUTER_INFO_MAX_LENGTH 150

// #define ROUTER_INFO_MIN_LENGTH (ROUTER_INFO_MAX_LENGTH - 32)

typedef struct router_t router_t;

typedef struct router_config_t router_config_t;

void router_init(router_t *router,router_config_t *config);

void router_sync(router_t *router);

typedef struct router_info_t router_info_t;

int router_get_info(router_t *router,keystore_nodeid_t nodeid,router_info_t *info);

void router_info_next(router_info_t *reader,uint8_t *buf,size_t nlen);

void router_info_append(router_info_t *reader,uint8_t *buf,size_t nlen);

int router_set_info(router_t *router,keystore_nodeid_t nodeid,router_info_t *info);

#endif
