#ifndef ROBSYS_APP_INTERNAL_IPC_TRANS_H
#define ROBSYS_APP_INTERNAL_IPC_TRANS_H
#include "utils/sensor/laser_beams_trigonometric_lookup_table.h"
#include "datacenter_local/datacenter_local.h"
#include "device_laserscan_livox_message/device_laserscan_livox_message.h"
#include "robot_metal_ipc_msg_bridge/robot_metal_ipc_msg_bridge.h"

#include "ipc/ipc_ssm.h"
#include "threads/thread.h"
namespace rob_sys {

class cIpcTrans: public datacenter::cDataCenterLocal
{
public:
    struct option_t {
        std::string _topic_laserscan;
        std::string _topic_livox_pts;
        std::string _topic_livox_imu;
        std::string _topic_rgbd_front;
    };
public:
    cIpcTrans(const std::string&);
    ~cIpcTrans();
public:
    int start();
    int stop();
public:
    int load_option_from_yaml(const std::string&,option_t&);
    void show_option(const option_t&);
private:
    void tfunc_grab_laserscan();
    void tfunc_grab_livox_pts();
    void tfunc_grab_livox_imu();
private:
    option_t _option;
private: //sub
    std::atomic<bool> _error;
    std::atomic<bool> _stop_request;
private:

    ipc::cStreamShareDataHandle<ipc_msg::cIpcMsgLasescan,ipc_msg::cIpcPropLaserscan>* _ipcsub_laserscan;
    ipc::cStreamShareDataHandle<ipc_msg::cIpcMsgLivoxCustom>*    _ipcsub_livox_pts;
    ipc::cStreamShareDataHandle<ipc_msg::cIpcMsgImu>*            _ipcsub_livox_imu;
private:
    datacenter::cDataCenterLocalPublisher<message::cMsgLaser,message::cPropLaser2D>* _pub_laser2d;
    datacenter::cDataCenterLocalPublisher<livox::cCustomMsg>* _pub_livox_pts;
    datacenter::cDataCenterLocalPublisher<message::cMsgIMU>*  _pub_livox_imu;
private:
    threads::cThreadPthread<std::function<void()>>* _th_laserscan;
    threads::cThreadPthread<std::function<void()>>* _th_livox_pts;
    threads::cThreadPthread<std::function<void()>>* _th_livox_imu;



private:
    sensor::cLaserBeamsGroup _beams_angle_table;
    message::cPropLaser2D _laser_prop;
};


}//namespace rob_sys
#endif//ROBSYS_APP_INTERNAL_IPC_TRANS_H
