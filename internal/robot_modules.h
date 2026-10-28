#ifndef ROBSYS_APP_INTERNAL_ROBOT_MODULES_H
#define ROBSYS_APP_INTERNAL_ROBOT_MODULES_H
#include "module/rs_module.h"
#include "module/rs_module_loader.h"
#include "module/rs_module_mgr.h"


#include "transform_server/transform_server.h"
#include "datacenter_local/datacenter_local.h"
namespace rob_sys {

class cRobotModules:public datacenter::cDataCenterLocal
{
public:
    using module_t = system::cRsModuleMgr::module_t;
public:
    explicit cRobotModules(const std::string&);
    ~cRobotModules();
public:
    int init();
    int start();
    int stop();
public:
    bool quick_exit();
private:
    int init_module();
    int init_service();
private:

    int show_list(const std::deque<module_t*>& modules);
    int load_common_list_from_yaml(const std::string& yaml,std::deque<module_t*>&,std::string& tf_srv_cfg);
private:
    std::string _module_cfgs;
    std::deque<module_t*> _modules;
    std::string _tf_srv_cfg;
    cTransformServer* _tf_srv;
private:
    std::atomic<bool> _quick_exit;
};

}//namespace rob_sys
#endif//ROBSYS_APP_INTERNAL_ROBOT_MODULES_H
