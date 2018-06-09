
#include "ice_rqst.h"
#include "ice_services.h"

using namespace NAMESPACE_GATEWAY::service;

NAMESPACE_GATEWAY_BEGIN

namespace rqst {

IceRqstBase::IceRqstBase(const std::string& rqstid, bool async)
  : rqstid_(rqstid), is_async_(async) {
}
IceRqstBase::IceRqstBase(const std::string& rqstid, const AMD_Srv_CallPtr& cb, bool async)
  : rqstid_(rqstid), callback_(cb), is_async_(async) {
}

IceRqstBase::~IceRqstBase() {
}

bool IceRqstBase::Start(const std::string& srv_id, const std::string& method, const std::string& rqst) {
    bool succ = Request();
    if(succ) {
        if(is_async_)
            succ = asyncCall(rqstid_, srv_id, method, rqst);
        else
            succ = syncCall(rqstid_, srv_id, method, rqst);
    }
    if(!succ) {
        std::string _resp = "{\"code\":1,\"message\":\"" + this->Error() + "\"}";
        this->response(false, _resp);
    }
    return succ;
}

void IceRqstBase::response(bool succ, const std::string& resp) {
    ProcessResult(succ, resp);
}
void IceRqstBase::exception(const Ice::Exception& ex) {
    std::string resp = "{\"code\": 1, \"message\": \"" + std::string(ex.ice_name()) + "\"}";
    ProcessResult(false, resp);
}
bool IceRqstBase::Request() { 
    return true; 
}
bool IceRqstBase::ProcessResult(bool succ, const std::string& resp) {
    return IceResponse(succ, resp);
}

bool IceRqstBase::IceResponse(bool succ, const std::string& resp) {
    if(callback_) {
        callback_->ice_response(succ, resp);
        return true;
    }
    return false;
}
bool IceRqstBase::IceResponse(const std::string& rqstid, const std::string& srv_id, const std::string& method, const std::string& rqst, bool succ, const std::string& resp, double elapse, AMD_Srv_CallPtr& callback) {
    if(callback) {
        callback->ice_response(succ, resp);
        return true;
    }
    return false;
}

bool IceRqstBase::asyncCall(const std::string& rqstid, const std::string& srv_id, const std::string& method, const std::string& rqst) {

    IceServices* services = IceServices::instance();
    auto _service = services->get(srv_id);
    if(!_service) {
        err_ = "service not exist " + srv_id;
        return false;
    }
    auto srv_prx = _service->srvPrx();
    if(!srv_prx) {
        err_ = "invalid SrvPrx " + srv_id;
        return false;
    }
    srv_prx->begin_Call(rqstid, method, rqst,
                        newCallback_Srv_Call(this, 
                            &IceRqstBase::response, 
                            &IceRqstBase::exception
                        ));
    return true;
}

bool IceRqstBase::syncCall(const std::string& rqstid, const std::string& srv_id, const std::string& method, const std::string& rqst) {

    IceServices* services = IceServices::instance();
    auto _service = services->get(srv_id);
    if(!_service) {
        err_ = "invalid service " + srv_id;
        return false;
    }
    auto srv_prx = _service->srvPrx();
    if(!srv_prx) {
        err_ = "invalid SrvPrx " + srv_id;
        return false;
    }
    try {
        std::string resp;
        bool succ = srv_prx->Call(rqstid, method, rqst, resp);
        this->response(succ, resp);
    } catch(const Ice::NoEndpointException& ex) {
        this->exception(ex);
    } catch(const IceUtil::InvalidTimeoutException& ex) {
        this->exception(ex);
    } catch(const Ice::LocalException& ex) {
        this->exception(ex);
    } catch(const Ice::Exception& ex) {
        this->exception(ex);
    }
    return true;
}

}

NAMESPACE_GATEWAY_END
