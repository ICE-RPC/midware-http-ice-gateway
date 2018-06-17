
#include "conf.h"

#include <iostream>
#include <libconfig.h++>

bool Config::load(const std::string& cfg_file) {

    libconfig::Config cfg;

    // Read the file. If there is an error, report it and exit.
    try  {
        cfg.readFile(cfg_file.c_str());
    } catch(const libconfig::FileIOException &fioex) {
        std::cerr << "I/O error while reading file." << std::endl;
        return false;
    } catch(const libconfig::ParseException &pex) {
        std::cerr << "Parse error at " << pex.getFile() << ":" << pex.getLine()
                << " - " << pex.getError() << std::endl;
        return false;
    }

    const libconfig::Setting& root = cfg.getRoot();

    try {

        const libconfig::Setting& server = root["services"];
        server.lookupValue("addr", http_addr_);
        server.lookupValue("port", http_port_);
        server.lookupValue("backlog", http_backlog_);
        server.lookupValue("threads", http_threads_);

        // ICE services
        const libconfig::Setting& services = root["services"];
        int count = services.getLength();
        for(int i=0; i<count; i++) {
            const libconfig::Setting& service = services[i];
            bool enabled = false;
            std::string srv_id, srv_name, srv_cfgfile;
            int srv_timeout = 500;

            if(!(service.lookupValue("enabled", enabled)
                && service.lookupValue("srv_id", srv_id)
                && service.lookupValue("srv_name", srv_name)
                && service.lookupValue("srv_cfg", srv_cfgfile)
                && service.lookupValue("srv_timeout", srv_timeout))) {
                continue;
            }
            if(!enabled || srv_id.empty() || srv_name.empty() || srv_cfgfile.empty() || srv_timeout < 0) {
                continue;
            }
            std::cerr<<"init service  "<<enabled<<" "<<srv_id<<" "<<srv_name<<" "<<srv_cfgfile<<" "<<srv_timeout<<std::endl;
            ice_config_service_t srv_cfg{enabled, srv_id, "SrvPrx", srv_name, srv_cfgfile, srv_timeout};
            ice_srv_configs_.emplace_back(std::move(srv_cfg));
        }

        // method -> srv_id configuration
        const libconfig::Setting& methods = root["method_srvid"];
        count = methods.getLength();
        for(int i=0; i<count; i++) {
            const libconfig::Setting& method_cfg = methods[i];
            std::string method, srv_id, srv_method;
            if(!(method_cfg.lookupValue("method", method)
                && method_cfg.lookupValue("srv_id", srv_id))) {
                continue;
            }
            if(method.empty() || srv_id.empty()) {
                continue;
            }
            method_cfg.lookupValue("srv_method", srv_method);
            if(srv_method.empty()) srv_method = method;

            std::cerr<<"init method->srv_id  "<<method<<" "<<srv_id<<" "<<srv_method<<std::endl;
            method_config_t method_srvid_cfg{srv_id, srv_method};
            method2srvid_[method] = method_srvid_cfg;
        }
    } catch(const libconfig::SettingNotFoundException &nfex) {
        std::cerr << "Parse error at " << nfex.what() << std::endl;
        return false;
    }

    return true;
}

