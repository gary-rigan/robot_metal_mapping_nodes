#include "stop_request.h"

namespace rob_sys {
namespace  {
static std::atomic<bool> _stop_request(false);
}//namespace

std::atomic<bool>& stop_request() {
    return _stop_request;
}

}//namespace rob_sys
