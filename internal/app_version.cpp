#include "app_version.h"

#include "strings/string_helper/string_macros.h"
#include <sstream>
namespace rob_sys {
namespace app {
namespace {

#ifdef ROBSYS_VERSION
#define PROJECT_VERSION ROBSYS_STRING_VAR_ARG_STRING(ROBSYS_VERSION);
#else
#define PROJECT_VERSION "v1.0.0";
#endif//ROBSYS_VERSION

#ifdef GIT_COMMIT_ID
#define PROJECT_COMMIT_ID ROBSYS_STRING_VAR_ARG_STRING(GIT_COMMIT_ID);
#else
#error "project commit-id not definded using default unknow"
#define PROJECT_COMMIT_ID "unknow";
#endif//GIT_COMMIT_ID
}//namespace


std::string version() {
    return PROJECT_VERSION;
}
std::string name() {
    return "robot_node_manager";
}
std::string info() {
    return "robot_node_manager";
}
std::string commit_id() {
    return PROJECT_COMMIT_ID;
}
std::string build_info() {
    std::stringstream ss;
    ss<<"Compile by:gcc["<<__VERSION__<<"] build on:"<<__DATE__<<" "<<__TIME__;
    return ss.str();
}



}//namespace app
}//namespace rob_sys
