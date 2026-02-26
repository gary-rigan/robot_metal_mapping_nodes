#include "process/robsys_process.h"
#include "exception/exception.h"

#include "stacktrace/stacktrace.h"
#include <signal.h>

#include "internal/stop_request.h"
#include "internal/robot_modules.h"
#include "internal/ipc_trans.h"
namespace  {


void sig_handle(int sig){
    LLOG(NOTICE,"Signal catched [%d],waitting for main thread stop ",sig);
    rob_sys::stop_request()=true;
}
void segmentation_fault(const char*) {
    LLOG(NOTICE,"Signal catched [SIGSEGV],in segmentation fault handle.try to stop main thread ");
    rob_sys::stop_request()=true;
}
}//namespace

int main(int argc,char* argv[])
{
#ifdef BUILD_BY_CONAN
    rob_sys::set_robsys_log_std   (true,INFO);
    rob_sys::set_robsys_log_syslog(false,INFO);
    rob_sys::set_robsys_log_file  (true,INFO,"/var/log/","rsnode_slam");
#endif//BUILD_BY_CONAN

    rob_sys::cRobsysApplication robsys_app(argc,argv);
    std::set<int> signals;
    signals.insert(SIGINT);
    //signals.insert(SIGSEGV);
    signals.insert(SIGTERM);
    signals.insert(SIGHUP);
    signals.insert(SIGABRT);

    robsys_app.regist_any_signal(signals,sig_handle);
    rob_sys::set_custom_handle_SIGSEGV(segmentation_fault);


    std::string module_cfg;
    std::string    ipc_cfg;

#ifdef BUILD_BY_CONAN
    module_cfg = "../config/robot_metal_mapping_nodes/config/rsnode_mapping_modules.yaml";
    ipc_cfg    = "../config/robot_metal_mapping_nodes/config/rsnode_mapping_ipc.yaml";
#else
    module_cfg = "/home/liyanx/workspace_robsys/robsys_robots/robot_metal_mapping_nodes/config/rsnode_mapping_modules.yaml";
       ipc_cfg = "/home/liyanx/workspace_robsys/robsys_robots/robot_metal_mapping_nodes/config/rsnode_mapping_ipc.yaml";
#endif//BUILD_BY_CONAN

    rob_sys::cIpcTrans ipc_trans(ipc_cfg);
    if(ipc_trans.start() != ROBSYS_SUCCESSED) {
        ipc_trans.stop();
        LLOG(ERROR,"Start ipc trans failed");
        robsys_exit(1);
    }

    rob_sys::cRobotMetalModules device_modues(module_cfg);
    if(device_modues.init()!=ROBSYS_SUCCESSED) {
        LLOG(ERROR,"Init kago5 device-modules failed");
        robsys_exit(-1);
    }
    if(device_modues.start() != ROBSYS_SUCCESSED) {
        LLOG(ERROR,"Start kago5 device-modules failed");
        robsys_exit(-1);
    }
    while (true) {
        if(rob_sys::stop_request()) {
            LLOG(WARNING,"Stop requested,in main func");
            break;
        }
        usleep(100000);
    }
    if(ipc_trans.stop() != ROBSYS_SUCCESSED) {
        LLOG(ERROR,"Stop ipc trans failed");
    }
    if(device_modues.stop() != ROBSYS_SUCCESSED) {
        LLOG(ERROR,"Stop modules failed");
    }
    robsys_exit(1);
    return 0;
}
