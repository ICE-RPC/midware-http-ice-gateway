
#pragma once

#include "namespace_def.h"

MODULE_GATEWAY_BEGIN
module idl {

interface Srv {

    ["amd"]
    idempotent bool Call(string rqstid, string method, string rqst, out string resp);

};

};
MODULE_GATEWAY_END
