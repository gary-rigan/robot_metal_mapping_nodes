#include "process/robsys_process.h"
#include "exception/exception.h"

#include "stacktrace/stacktrace.h"
#include <signal.h>

#include "internal/stop_request.h"
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
    return 0;
}
