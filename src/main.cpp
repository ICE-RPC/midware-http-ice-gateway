
#include <string>

#include "evhtp/evhtp.h"

#include "ice_services.h"
#include "ice_resp_handler.h"
#include "http_handler.h"

#include "conf.h"

using namespace NAMESPACE_GATEWAY::service;

struct app_info {
    evbase_t* evbase;
    evhtp_t* evhtp;
};

struct thread_info {
    struct app_info* parent;
    evbase_t* evbase;
};

void gateway_init_thread(evhtp_t* evhtp, evthr_t* thread, void* arg) {

    struct app_info* parent = (struct app_info*)arg;
    struct thread_info* info = (struct thread_info*)calloc(sizeof(struct thread_info), 1);

    info->parent = parent;
    info->evbase = evthr_get_base(thread);

    evthr_set_aux(thread, info);
}

int main(int argc, char ** argv) {

    std::string cfg_file = "config.json";
    if(argc > 1) {
        cfg_file = argv[1];
    }
    Config& config = Config::instance();
    if(!config.load(cfg_file)) {
        std::cerr<<"config init failed "<<cfg_file<<std::endl;
        return EXIT_FAILURE;
    }

    // 初始化ice服务
    std::string err{"success"};
    const std::vector<struct ice_config_service_t>& ice_srv_configs = config.getIceSrvConfigs();
    struct ice_config_services_t srv_configs;
    for(auto& ice_srv_conf : ice_srv_configs) {
        srv_configs.services[ice_srv_conf.srv_id] = ice_srv_conf;
    }
    int res = IceAsyncHttpRespHandler::init(srv_configs, err);
    if(res != 0) {
        std::cerr<<res<<" "<<err<<std::endl;
    }

    int http_threads    = config.getThreads();
    std::string baddr   = config.getHttpAddr();
    uint16_t bport      = config.getHttpPort();
    int backlog         = config.getHttpBacklog();
    int nodelay         = config.getHttpNodelay();
    int defer_accept    = config.getHttpDeferAccept();
    int reuse_port      = config.getHttpReusePort();

    struct event_base* evbase = event_base_new();
    evhtp_t* evhtp = evhtp_new(evbase, NULL);

    evhtp_set_parser_flags(evhtp, EVHTP_PARSE_QUERY_FLAG_LENIENT);
    if(nodelay)
        evhtp_enable_flag(evhtp, EVHTP_FLAG_ENABLE_NODELAY);

    if(defer_accept)
        evhtp_enable_flag(evhtp, EVHTP_FLAG_ENABLE_DEFER_ACCEPT);

    if(reuse_port)
        evhtp_enable_flag(evhtp, EVHTP_FLAG_ENABLE_REUSEPORT);

    for(request_handler_t* p=request_handlers; p && p->path; p++) {
        evhtp_set_cb(evhtp, p->path, p->handler, p->arg);
    }

#ifndef EVHTP_DISABLE_EVTHR
    if(http_threads > 0) {
        struct app_info* app_p = new struct app_info;
        app_p->evbase = evbase;
        app_p->evhtp = evhtp;
        evhtp_use_threads_wexit(evhtp, gateway_init_thread, NULL, http_threads, app_p);
    }
#endif
    evhtp_bind_socket(evhtp, baddr.c_str(), bport, backlog);
    event_base_loop(evbase, 0);

    return EXIT_SUCCESS;
} /* main */

