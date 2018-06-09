
#include "conf.h"

bool Config::load(const std::string& cfg_file) {

    // TODO: read from cfg_file


    // ICE服务配置
    ice_srv_configs_ = std::vector<struct ice_config_service_t>{
        {true, "midware-ice-service-001", "SrvPrx", "midware-ice-service", "ice.admin.conf", 100},
        {true, "midware-ice-service-002", "SrvPrx", "midware-ice-service", "ice.admin.conf", 100},
    };

    // method -> srv_id
    method2srvid_ = std::unordered_map<std::string, struct method_config_t>{
        {"api.worldcup.matches", {"midware-ice-service-001", "worldcup.matches"}},
        {"api.worldcup.match", {"midware-ice-service-002", "worldcup.match"}},
    };

    return true;
}

