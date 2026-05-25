#ifndef ROBSYS_APP_APP_VERSION_H
#define ROBSYS_APP_APP_VERSION_H
#include <string>
namespace rob_sys {
namespace app {

extern std::string   version();
extern std::string      name();
extern std::string      info();
extern std::string commit_id();
extern std::string build_info();

}//namespace app
}//namespace rob_sys

#endif//ROBSYS_APP_APP_VERSION_H
