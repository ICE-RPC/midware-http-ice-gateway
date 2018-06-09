
#include "http_handler.h"

#include <string>

#include "utils.h"
#include "timer.h"

#include "conf.h"
#include "ice_resp_handler.h"

using namespace NAMESPACE_GATEWAY::rqst;
using namespace NAMESPACE_GATEWAY::service;

static const char* err_method_resp = "{\"code\":1,\"message\":\"invalid method\"}";

void router_request_handler(evhtp_request_t* r, void* arg) {

    TIMER(tv_start);

    const evhtp_kv_t* param_method = evhtp_kvs_find_kv(r->uri->query, "method");
    if(param_method == NULL) {
        evbuffer_add_printf(r->buffer_out, "%s", err_method_resp);
        evhtp_send_reply(r, EVHTP_RES_BADREQ);
        return;
    }
    //const char* v = evhtp_header_find(r->headers_in, "Host");  // 忽略大小写

    std::string rqst{""};
    size_t body_len = evbuffer_get_length(r->buffer_in);
    const char* body = (const char *)evbuffer_pullup(r->buffer_in, body_len);
    if(body_len >= 0 && body != nullptr) {
        rqst = std::string(body, body_len);
    }

    std::string method{param_method->val, param_method->vlen};

    std::string rqstid = RQSTID();

    // method -> srv_id
    struct method_config_t method_cfg;
    const Config& config = Config::instance();
    int res = config.getSrvId(method, method_cfg);
    if(res != 0 || method_cfg.srv_id.empty()) {
        evbuffer_add_printf(r->buffer_out, "%s", err_method_resp);
        evhtp_send_reply(r, EVHTP_RES_BADREQ);
        return;
    }
    if(method_cfg.srv_method.empty()) {
        method_cfg.srv_method = method;
    }

    // 根据配置路由异步(也可同步)调用下游服务(通用), 直接返回下游服务的数据
    IceAsyncHttpRespHandler::Call(rqstid, method_cfg.srv_id, method_cfg.srv_method, rqst, true, tv_start, r);
}

void ping_request_handler(evhtp_request_t* r, void* arg) {
    static const char* p = "PONG";
    evbuffer_add_reference(r->buffer_out, (const char *)p, strlen(p), NULL, NULL);
    evhtp_send_reply(r, EVHTP_RES_OK);
}
