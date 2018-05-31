#ifndef COAP_H
#define COAP_H

#include "lobaro-coap/src/coap.h"
#include "ascl/ascl.h"
#include "ascl/udp.h"

#define COAP_MEMORY_SIZE 4096

typedef struct coap_t coap_t;

struct coap_t {
    CoAP_API_t api;
    CoAP_Config_t config;
    ascl_udp_t *udp;
    CoAP_Socket_t *socket;
    uint8_t memory[COAP_MEMORY_SIZE];
};

void coap_init(ascl_t *ascl, coap_t *coap);

void coap_create_reources(char* Uri, char* Descr, CoAP_ResOpts_t Options, CoAP_ResourceHandler_fPtr_t pHandlerFkt, CoAP_ResourceNotifier_fPtr_t pNotifierFkt);

void coap_get_request(char* UriString, NetEp_t* ServerEp, CoAP_RespHandler_fn_t cb);

void coap_post_request(char *UriString, NetEp_t *ServerEp, CoAP_RespHandler_fn_t cb);

#endif
