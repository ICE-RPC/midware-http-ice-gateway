
#pragma once

#include <string>
#include <functional>

#include <ice_interface.h>

#include "namespace_def.h"

using namespace NAMESPACE_GATEWAY::idl;

NAMESPACE_GATEWAY_BEGIN

namespace rqst {

class IceRqstBase : public IceUtil::Shared {
public:
    IceRqstBase(const std::string& rqstid, bool async=true);
    IceRqstBase(const std::string& rqstid, const AMD_Srv_CallPtr& cb, bool async=true);
    virtual ~IceRqstBase();

    const std::string& RqstId() { return rqstid_; }

    const std::string& Error() { return err_; }
    void Error(const std::string& err) { err_ = err; }

    bool Start(const std::string& srv_id, const std::string& method, const std::string& rqst);

    bool IceResponse(bool succ, const std::string& resp);
    static bool IceResponse(const std::string& rqstid, const std::string& srv_id, const std::string& method, const std::string& rqst, bool succ, const std::string& resp, double elapse, AMD_Srv_CallPtr& callback);

    // 可以继承的2个函数
    virtual bool Request();
    virtual bool ProcessResult(bool succ, const std::string& resp);

    void response(bool succ, const std::string& resp);
    void exception(const Ice::Exception& ex);

protected:
    bool asyncCall(const std::string& rqstid, const std::string& srv_id, const std::string& method, const std::string& rqst);
    bool syncCall(const std::string& rqstid, const std::string& srv_id, const std::string& method, const std::string& rqst);


protected:
    bool is_async_{true};

    std::string rqstid_;
    std::string err_;
    AMD_Srv_CallPtr callback_;
};
typedef IceUtil::Handle<IceRqstBase> IceAsyncCallbackHandlerPtr;

}

NAMESPACE_GATEWAY_END
