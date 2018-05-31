#include "coap.h"

static void tx(SocketHandle_t socketHandle, NetPacket_t* pckt){
    
}

static uint32_t rtc(void) {

}

static void debug(char* s) {

}

void coap_init(ascl_t *ascl, coap_t *coap) {
    CoAP_Init(coap->api,coap->config);
    coap->udp = ascl_udp_init(ascl);
    coap->api.Memory = coap->memory;
    coap->api.MemorySize = COAP_MEMORY_SIZE;
    coap->socket = CoAP_NewSocket(coap->udp);
    coap->socket->Tx = tx;
    coap->api->rtc1HzCnt = rtc;
    coap->api->debugPuts = debug;
}

void coap_create_reources(char* Uri, char* Descr, CoAP_ResOpts_t Options, CoAP_ResourceHandler_fPtr_t pHandlerFkt, CoAP_ResourceNotifier_fPtr_t pNotifierFkt);

void coap_get_request(char* UriString, NetEp_t* ServerEp, CoAP_RespHandler_fn_t cb);

void coap_post_request(char *UriString, NetEp_t *ServerEp, CoAP_RespHandler_fn_t cb);



