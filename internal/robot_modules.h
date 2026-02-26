#ifndef ROBSYS_APP_INTERNAL_ROBOT_MODULES_H
#define ROBSYS_APP_INTERNAL_ROBOT_MODULES_H


#include "internal/module_list.h"
#include "transform_server/transform_server.h"
#include "datacenter_local/datacenter_local.h"
namespace rob_sys {

class cRobotMetalModules:public datacenter::cDataCenterLocal
{
public:
    explicit cRobotMetalModules(const std::string&);
    ~cRobotMetalModules();
public:
    int init();
    int start();
    int stop();

private:
    int init_module();
    int init_service();
private:
    std::string _module_cfgs;
    std::deque<module_t*> _modules;
    std::string _tf_srv_cfg;
    cTransformServer* _tf_srv;
};

}//namespace rob_sys
#endif//ROBSYS_APP_INTERNAL_ROBOT_MODULES_H
