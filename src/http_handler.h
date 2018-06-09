
#pragma once

#include "evhtp/evhtp.h"

void ping_request_handler(evhtp_request_t* r, void* a);
void router_request_handler(evhtp_request_t* r, void* arg);

typedef void (*_evhtp_request_handler_)(evhtp_request_t* r, void *arg);
struct request_handler_t {
    const char* path;
    _evhtp_request_handler_ handler;
    void* arg;
};

static request_handler_t request_handlers [] = { 
    { "/ping", ping_request_handler, NULL },
    { "/router", router_request_handler, NULL },
    { NULL, NULL, NULL},
};
