
#pragma once

#include <string>
#include <unordered_map>

#include <Ice/Ice.h>
#include <ice_interface.h>

#include "namespace_def.h"

NAMESPACE_GATEWAY_BEGIN
namespace service {

struct ice_config_service_t {
    bool srv_enable;
    std::string srv_id;
    std::string srv_class;
    std::string srv_name;
    std::string srv_addr;
    int32_t srv_timeout;
};

struct ice_config_services_t {
    std::unordered_map<std::string, struct ice_config_service_t> services;  // readonly after inited
};


class IceService {
public:

    IceService() {}
    virtual ~IceService() {}

    virtual int init(const struct ice_config_service_t& conf, std::string& err) {

        bool srv_enable = conf.srv_enable;
        const std::string& srv_id = conf.srv_id;
        const std::string& srv_name = conf.srv_name;
        const std::string& srv_addr = conf.srv_addr;
        int srv_timeout = conf.srv_timeout;
        if(!srv_enable) {
            err = srv_id + " disabled";
            return -1;
        }
        if(srv_id.empty() || srv_name.empty() || srv_addr.empty()) {
            err = "srv_id, srv_name or srv_addr invalid";
            return -1;
        }

        std::string ice_conf = "--Ice.Config=" + srv_addr;
        char* argv[] = { (char*)ice_conf.c_str() };
        int argc = sizeof(argv)/sizeof(argv[0]);

        const std::string rqstid = "INIT";

        try {
            Ice::CommunicatorPtr ic = Ice::initialize(argc, argv);
            if(!ic) {
                err = "create channel failed srv_id=" + srv_id;
                return -1;
            }
            // use uncheckedCast, checkedCast:当服务不存在或者异常时会导致启动失败
            srv_prx_ = NAMESPACE_GATEWAY::idl::SrvPrx::uncheckedCast(ic->stringToProxy(srv_name));
            if(!srv_prx_) {
                err = "connect to service failed srv_id " + srv_id;
                return -1;
            }
            if(srv_timeout > 0)
                srv_prx_ = srv_prx_->ice_invocationTimeout(srv_timeout);
            srv_prx_ = srv_prx_->ice_connectionCached(false) 
                               ->ice_locatorCacheTimeout(300);

            return 0;

        } catch(const Ice::NoEndpointException& ex) {
            err = " Exception " + std::string(ex.what());
        } catch(const IceUtil::InvalidTimeoutException& ex) {
            err = " Exception " + std::string(ex.what());
        } catch(const Ice::LocalException& ex) {
            err = " Exception " + std::string(ex.what());
        } catch(const Ice::Exception& ex) {
            err = " Exception " + std::string(ex.what());
        }
        return -2;
    }

    struct ice_config_service_t srvConf() { return srv_conf_; }
    NAMESPACE_GATEWAY::idl::SrvPrx srvPrx() { return srv_prx_; }

private:
    struct ice_config_service_t srv_conf_;
    NAMESPACE_GATEWAY::idl::SrvPrx srv_prx_;
};

class IceServices {
public:
    static IceServices* instance() {
        static IceServices services;
        return &services;
    }

    int init(const struct ice_config_services_t& srv_configs, std::string& err);

    IceService* get(const std::string& srv_id);

protected:
    int createChannel(const struct ice_config_service_t& srv_conf, std::string& err);

private:
    std::unordered_map<std::string, IceService*> channels_;

private:
    IceServices() {}
};

}
NAMESPACE_GATEWAY_END
