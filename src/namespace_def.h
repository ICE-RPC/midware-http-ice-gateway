
#pragma once

#define NAMESPACE_GATEWAY_BEGIN \
namespace com {                 \
  namespace shamaoge {          \
    namespace gateway {
    
#define NAMESPACE_GATEWAY_END   \
    }                           \
  }                             \
}

#define NAMESPACE_GATEWAY com::shamaoge::gateway


#define MODULE_GATEWAY_BEGIN    \
module com {                    \
  module shamaoge {             \
    module gateway {
 
#define MODULE_GATEWAY_END      \
    };                          \
  };                            \
};
 
