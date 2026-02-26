#include "robot_modules.h"
#include "file_system/file_system.h"
#include "LLog/llogs.h"
namespace rob_sys {

cRobotMetalModules::cRobotMetalModules(const std::string& cfg)
    : _module_cfgs(cfg),
      _tf_srv(nullptr)
{
}
cRobotMetalModules::~cRobotMetalModules() {
    cTransformServer::release(_tf_srv);
}
int cRobotMetalModules::init_module() {

    std::stringstream ss;
    ss<<"\nModule: ";
    for(size_t i=0;i<_modules.size();++i) {
        SLOG(INFO)<<"Initializing module : ";
        auto* module = _modules[i];
        module->module = module->loader.load(module->module_lib);
        if(module->module == nullptr) {
            LLOG(ERROR,"Load robsys-module failed");
            return ROBSYS_ERROR;
        }
        module->module_impl = module->module->generate(module->module_cfg);
        if(module->module_impl == nullptr) {
            LLOG(ERROR,"Create module impl results null");
            return ROBSYS_ERROR;
        }
        ss <<"\n>\tImpl :"   <<module->module_impl->name()
          <<"\n>\tVersion :"<<module->module_impl->version();
    }
    SLOG(INFO)<<ss.str()<<"\nLoaded";
    return ROBSYS_SUCCESSED;
}
int cRobotMetalModules::init_service() {

    if(!File::access_file(_tf_srv_cfg.c_str())) {
        LLOG(ERROR,"Init srv failed,cfg file [%s] does not exist",_tf_srv_cfg.c_str());
        return ROBSYS_ERROR;
    }
    _tf_srv = cTransformServer::generate_handle3d(_tf_srv_cfg);
    if(_tf_srv == nullptr) {
        LLOG(ERROR,"Generate tf srv results null");
        return ROBSYS_ERROR;
    }
    if(!this->regist_plugin<cTransformServer>("metal-tf-srv",_tf_srv)) {

        LLOG(ERROR,"Regist transform srv plugin failed");
        return ROBSYS_ERROR;
    }
    return ROBSYS_SUCCESSED;
}
int cRobotMetalModules::init() {
    if(load_common_list_from_yaml(_module_cfgs,_modules,_tf_srv_cfg)!= ROBSYS_SUCCESSED) {
        LLOG(FATAL,"Load list from yaml failed");
    }
    show_list(_modules);
    if(!_tf_srv_cfg.empty()) {
        if(init_service() != ROBSYS_SUCCESSED) {
            LLOG(ERROR,"Init service failed");
            return ROBSYS_ERROR;
        }
    }
    if(init_module() != ROBSYS_SUCCESSED) {
        LLOG(ERROR,"Init module failed");
        return ROBSYS_ERROR;
    }
    return ROBSYS_SUCCESSED;
}
int cRobotMetalModules::start() {

    for(size_t i=0;i<_modules.size();++i) {
        SLOG(INFO)<<"Initializing module : ";
        auto* module = _modules[i];
        if(module->module_impl == nullptr) {
            LLOG(ERROR,"Create module impl results null");
            return ROBSYS_ERROR;
        }
        if(!module->module_impl->start()) {
            LLOG(ERROR,"Start module failed");
            return ROBSYS_ERROR;
        }
    }
    return ROBSYS_SUCCESSED;
}
int cRobotMetalModules::stop() {
    for(size_t i=0;i<_modules.size();++i) {
        SLOG(INFO)<<"Stoppin module.. ";
        auto* module = _modules[i];
        LLOG(INFO,"Stopping module [%s]",module->module_impl->name());
        if(!module->module_impl->stop()) {
            LLOG(ERROR,"Stop module impl [%s] failed",module->module_impl->name());
            return ROBSYS_ERROR;
        }
        LLOG(INFO,"Module [%s] stoped",module->module_impl->name());

    }
    return ROBSYS_SUCCESSED;
}
}//namespace rob_sys
