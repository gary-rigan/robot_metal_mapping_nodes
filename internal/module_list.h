#ifndef ROBSYS_APP_INTERNAL_MODULE_LIST_H
#define ROBSYS_APP_INTERNAL_MODULE_LIST_H

#include "module/rs_module.h"
#include "module/rs_module_loader.h"
namespace rob_sys {

struct module_t {
public:
    std::string module_lib;
    std::string module_cfg;
public:
    ~module_t();
public:
    system::cRobsysModuleLoader loader;
    system::cRobsysModuleFactory<std::string>* module=nullptr;
    system::cRobsysModuleBase*  module_impl =nullptr;
};

extern int show_list(const std::deque<module_t*>& modules);
extern int load_common_list_from_yaml(const std::string& yaml,std::deque<module_t*>&,std::string& tf_srv_cfg);
}//namespace rob_sys
#endif//ROBSYS_APP_INTERNAL_MODULE_LIST_H
