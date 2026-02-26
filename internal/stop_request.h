#ifndef ROBSYS_APP_INTERNAL_STOP_REQUEST_H
#define ROBSYS_APP_INTERNAL_STOP_REQUEST_H

#include <atomic>
namespace rob_sys {

extern std::atomic<bool>& stop_request();

}//namespace rob_sys
#endif//ROBSYS_APP_INTERNAL_STOP_REQUEST_H
