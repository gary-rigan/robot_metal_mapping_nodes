#include "robot_modules.h"

#include "file_system/file_system.h"
#include "robsys_parsers/robsys_parsers.h"



namespace rob_sys {
namespace  {

bool check_module_path(cRobotModules::module_t* module) {


    if(!File::access_file(module->module_lib.c_str())) {
        LLOG(ERROR,"Module file [%s] does not exist",module->module_lib.c_str());
        return false;
    }
    if(!module->module_cfg.empty()) {
        if(!File::access_file(module->module_cfg.c_str())) {
            LLOG(ERROR,"Module config [%s] does not exist",module->module_cfg.c_str());
            return false;
        }
    }
    return true;
}
}//namespace

int cRobotModules::show_list(const std::deque<module_t*>& modules) {
    std::stringstream ss;
    ss<<"\nModules:";
    for(size_t i=0;i<modules.size();++i) {
        ss<<"\nID: "<<i
         <<"\n>Lib:"<<modules[i]->module_lib
         <<"\n>Cfg:"<<modules[i]->module_cfg;
    }
    SLOG(INFO)<<ss.str();
    return ROBSYS_SUCCESSED;
}
int cRobotModules::load_common_list_from_yaml(const std::string& yaml,std::deque<module_t*>& mods,std::string& tf_srv_cfg) {

    YAML::Node node = YAML::LoadFile(yaml);
    if(node.IsNull()) {
        LLOG(ERROR,"Load yaml file failed");
        return ROBSYS_ERROR;
    }
    std::string lib_path= node["Library_path"].as<std::string>() + "/";
    YAML::Node nmodules = node["Modules"];
    if(nmodules.IsNull()) {
        LLOG(ERROR,"Cannot found modules in yaml file");
        return ROBSYS_ERROR;
    }
    if(!nmodules.IsSequence()) {
        LLOG(ERROR,"Modules data in yaml node type error,sequence expected");
        return ROBSYS_ERROR;
    }
    bool failed= false;
    for(size_t i=0;i<nmodules.size();++i) {
        auto nm = nmodules[i];

        module_t* module = new module_t();
        module->module_cfg = nm[1]["module_cfg"].as<std::string>();
        module->module_lib = lib_path + "/" +nm[0]["module_lib"].as<std::string>().c_str();

        failed = !check_module_path(module);
        if(failed) {
            LLOG(ERROR,"Check module lib and cfg file failed");
            delete module;
            break;
        }
        mods.push_back(module);
    }
    if(node["TF_srv_cfg"].IsDefined()) {
        tf_srv_cfg = node["TF_srv_cfg"].as<std::string>();
    }
    else {
        tf_srv_cfg =std::string();
        tf_srv_cfg.clear();
    }
    _quick_exit = false;
    if(node["Quick_exit"].IsDefined()) {
        _quick_exit = node["Quick_exit"].as<bool>();
    }
    return failed?ROBSYS_ERROR:ROBSYS_SUCCESSED;
}
}//namespace rob_sys
