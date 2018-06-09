
#include <iostream>

#include "ice_services.h"

NAMESPACE_GATEWAY_BEGIN

namespace service {

int IceServices::init(const struct ice_config_services_t& srv_configs, std::string& err) {
    if(srv_configs.services.empty()) {
        err = "services is empty";
        return -1;
    }

    int succ = 0, errors = 0;
    for(auto& pair : srv_configs.services) {
        const auto& srv_conf = pair.second;
        std::string srv_err;
        int res = createChannel(srv_conf, srv_err);
        if(res != 0) {
            ++errors;
            err += srv_err + ";";
            continue;
        }
        ++succ;
    }
    return errors>0?-1:0;
}

int IceServices::createChannel(const struct ice_config_service_t& srv_conf, std::string& err) {
    std::string srv_class = srv_conf.srv_class;
    IceService* srv = new IceService;
    if(srv == nullptr) {
        err = "allocate service failed";
        return -1;
    }
    int res = srv->init(srv_conf, err);
    if(res != 0) {
        if(srv) delete srv;
        return res;
    }
    channels_[srv_conf.srv_id] = srv;

    return 0;
}

IceService* IceServices::get(const std::string& srv_id) {
    auto iter = channels_.find(srv_id);
    if(iter == channels_.end()) {
        return nullptr;
    }
    return iter->second;
}

}
NAMESPACE_GATEWAY_END
