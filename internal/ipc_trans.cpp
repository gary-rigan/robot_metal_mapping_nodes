#include "ipc_trans.h"
#include "internal/stop_request.h"
namespace rob_sys {


cIpcTrans::cIpcTrans(const std::string& cfg)
    : _error(false),
      _stop_request(false),
      _ipcsub_laserscan(nullptr),
      _ipcsub_livox_pts(nullptr),
      _ipcsub_livox_imu(nullptr),

      _pub_laser2d(nullptr),_pub_livox_pts(nullptr),_pub_livox_imu(nullptr),

      _th_laserscan(nullptr),_th_livox_pts(nullptr),_th_livox_imu(nullptr)
{

    if( load_option_from_yaml(cfg,_option) != ROBSYS_SUCCESSED) {
        LLOG(FATAL,"Load option from yaml failed");
    }
    show_option(_option);
}
cIpcTrans::~cIpcTrans() {
    //ipc handle
    if(_ipcsub_laserscan) {
        delete _ipcsub_laserscan;
        _ipcsub_laserscan = nullptr;
    }
    if(_ipcsub_livox_pts) {
        delete _ipcsub_livox_pts;
        _ipcsub_livox_pts = nullptr;
    }
    if(_ipcsub_livox_imu) {
        delete _ipcsub_livox_imu;
        _ipcsub_livox_imu = nullptr;
    }
    //local handle
    if(_pub_laser2d) {
        delete _pub_laser2d;
        _pub_laser2d =nullptr;
    }
    if(_pub_livox_pts) {
        delete _pub_livox_pts;
        _pub_livox_pts =nullptr;
    }
    if(_pub_livox_imu) {
        delete _pub_livox_imu;
        _pub_livox_imu = nullptr;
    }
}

//_th_laserscan(nullptr),_th_livox_pts(nullptr),_th_livox_imu(nullptr)
int cIpcTrans::start() {
    _stop_request = false;
    _error = false;
    //local handle
    _pub_laser2d   = create_publisher<message::cMsgLaser,message::cPropLaser2D>(_option._topic_laserscan);
    _pub_livox_pts = create_publisher<livox::cCustomMsg>                       (_option._topic_livox_pts);
    _pub_livox_imu = create_publisher<message::cMsgIMU>                        (_option._topic_livox_imu);


    _th_laserscan = new threads::cThreadPthread<std::function<void()>>(std::bind(&cIpcTrans::tfunc_grab_laserscan,this));
    if(!_th_laserscan) {
        _error = true;
        LLOG(ERROR,"Create laserscan grab thread failed");
        return ROBSYS_ERROR;
    }
    _th_livox_pts = new threads::cThreadPthread<std::function<void()>>(std::bind(&cIpcTrans::tfunc_grab_livox_pts,this));
    if(!_th_livox_pts) {
        _error = true;
        LLOG(ERROR,"Create livox-pts grab thread failed");
        return ROBSYS_ERROR;
    }

    _th_livox_imu = new threads::cThreadPthread<std::function<void()>>(std::bind(&cIpcTrans::tfunc_grab_livox_imu,this));
    if(!_th_livox_imu) {
        _error = true;
        LLOG(ERROR,"Create livox-pts grab thread failed");
        return ROBSYS_ERROR;
    }


    _ipcsub_laserscan = new ipc::cStreamShareDataHandle<ipc_msg::cIpcMsgLasescan,ipc_msg::cIpcPropLaserscan>(_option._topic_laserscan.c_str(),0);
    if(_ipcsub_laserscan ==nullptr) {
        LLOG(ERROR,"Alloc ipc handle for [Laserscan2d] results null");

    }
    _ipcsub_laserscan->set_blocking(true);


    _ipcsub_livox_pts = new ipc::cStreamShareDataHandle<ipc_msg::cIpcMsgLivoxCustom>(_option._topic_livox_pts.c_str(),0);
    if(_ipcsub_livox_pts == nullptr) {
        LLOG(ERROR,"Alloc ipc handle for [Livox-pts] results null");
    }
    _ipcsub_livox_pts->set_blocking(true);


    _ipcsub_livox_imu = new ipc::cStreamShareDataHandle<ipc_msg::cIpcMsgImu>        (_option._topic_livox_imu.c_str(),0);
    if(_ipcsub_livox_imu == nullptr) {
        LLOG(ERROR,"Alloc ipc handle for [Livox-imu] results null");

    }
    _ipcsub_livox_imu->set_blocking(true);

    while (true) {
        if(stop_request()) {
            LLOG(WARNING,"Stop requested!");
            return ROBSYS_ERROR;
        }
        if(!ipc::init_ssm()) {
            LLOG(ERROR,"Init ipc ssm handle failed,retry ...");
            times::sleep_sys_t::rs_sleep(1.0);
        }
        else {
            LLOG(INFO,"SSM successfully opened!");
            break;
        }
    }
    bool laserscan_opened = false;
    bool livox_imu_opened = false;
    bool livox_pts_opened = false;

    while (true) {
        if(_stop_request) {
            LLOG(WARNING,"Stop requested");
            break;
        }
        if(!laserscan_opened) {
            if(!_ipcsub_laserscan->open(ipc::SSM_READ) ) {
                LLOG(ERROR,"Open laserscan2d ipc handle failed");
            }
            else {
                laserscan_opened = true;
            }
        }
        if(!livox_imu_opened) {
            if(!_ipcsub_livox_imu->open(ipc::SSM_READ) ) {
                LLOG(ERROR,"Open livox-imu ipc handle failed");
            }
            else {
                livox_imu_opened = true;
            }

        }
        if(!livox_pts_opened) {
            if(!_ipcsub_livox_pts->open(ipc::SSM_READ) ) {
                LLOG(ERROR,"Open livox-pts ipc handle failed");
            }
            else {
                livox_pts_opened = true;

            }

        }

        if(laserscan_opened && livox_imu_opened && livox_pts_opened) {
            LLOG(NOTICE,"All ipc puslisher opened");
            break;
        }
        times::sleep_sys_t::rs_sleep(0.5);
    }

    if(_th_laserscan->thread_start() != ROBSYS_SUCCESSED) {
        LLOG(ERROR,"Start laserscan grab thread failed");
    }
    if(_th_livox_pts->thread_start() != ROBSYS_SUCCESSED) {
        LLOG(ERROR,"Start livox-pts grab thread failed");
    }
    if(_th_livox_imu->thread_start() != ROBSYS_SUCCESSED) {
        LLOG(ERROR,"Start livox-imu grab thread failed");
    }

    return ROBSYS_SUCCESSED;
}


int cIpcTrans::stop() {
    SLOG(INFO)<<"Stopping ipc trans ...";
    _stop_request = true;


    if(_th_laserscan) {
        SLOG(INFO)<<"Grab [laserscan] thread joined ...";
        _th_laserscan->thread_join();
        delete _th_laserscan;
        _th_laserscan = nullptr;
    }
    if(_th_livox_imu) {
        SLOG(INFO)<<"Grab [livox-imu] thread joined ...";
        _th_livox_imu->thread_join();
        delete _th_livox_imu;
        _th_livox_imu = nullptr;
    }
    if(_th_livox_pts) {

        SLOG(INFO)<<"Grab [livox-pts] thread joined ...";
        _th_livox_pts->thread_join();
        delete _th_livox_pts;
        _th_livox_pts = nullptr;
    }


    if(_ipcsub_laserscan) {
        if(!_ipcsub_laserscan->close()) {
            LLOG(ERROR,"Close ipc handle [Laserscan2d] failed");
        }
        delete _ipcsub_laserscan;
        _ipcsub_laserscan = nullptr;
    }
    if(_ipcsub_livox_pts) {
        if(!_ipcsub_livox_pts->close()) {
            LLOG(ERROR,"Close ipc handle [Livox-pts] failed");
        }
        delete _ipcsub_livox_pts;
        _ipcsub_livox_pts = nullptr;
    }
    if(_ipcsub_livox_imu) {
        if(!_ipcsub_livox_imu->close()) {
            LLOG(ERROR,"Close ipc handle [Livox-imu] failed");
        }
        delete _ipcsub_livox_imu;
        _ipcsub_livox_imu = nullptr;
    }


    if(!ipc::end_ssm()) {
        LLOG(ERROR,"End ssm failed");
    }



    if(_pub_livox_pts) {
        delete _pub_livox_pts;
        _pub_livox_pts = nullptr;
    }
    if(_pub_livox_imu) {
        delete _pub_livox_imu;
        _pub_livox_imu = nullptr;
    }

    if(_pub_laser2d) {
        delete _pub_laser2d;
        _pub_laser2d =nullptr;
    }
    return ROBSYS_SUCCESSED;
}

void cIpcTrans::tfunc_grab_laserscan() {
    if(_ipcsub_laserscan == nullptr) {
        LLOG(ERROR,"Start laserscan grab thread failed,ipc handle is null");
        _error = true;
        return ;
    }

    while (true) {
        if(!_ipcsub_laserscan->get_property()) {
            LLOG(WARNING,"Get laserscan2d property from ipc failed,retry");
        }
        else {
            LLOG(INFO,"Laserscan2d property got");
            break;
        }

        if(_stop_request) {
            LLOG(WARNING,"Stop requested");
            return ;
        }
        sleep(1);
    }

    if(!ipc_msg::to_robsys_msg(_ipcsub_laserscan->property,_laser_prop)) {
        LLOG(FATAL,"Convert ipc-msg [laserscan2d-property] to robsys msg failed");
        _error = true;
    }

    SLOG(INFO)<<"Creating laser beam angle lookup-table ...";

    if(!sensor::cLaserBeamsTrigonometricLookupTable::create(_laser_prop._beams,
                                                            _laser_prop._center_index,_laser_prop._angle_per_line,
                                                            _beams_angle_table)) {
        robsys_runtime_error("Create laser beams angle lookup table failed");
    }

    if(!sensor::cLaserBeamsStampsSubdivisionLookupTable::create(_laser_prop._beams,
                                                                _laser_prop._frame_time_incresement,_beams_angle_table)) {
        robsys_runtime_error("Create laser beams stamp lookup table failed");
    }
    _laser_prop._angle_lookup_table      = _beams_angle_table.rads;
    _laser_prop._angle_cos_lookup_table  = _beams_angle_table.coss;
    _laser_prop._angle_sin_lookup_table  = _beams_angle_table.sins;
    _laser_prop._beam_stamps_lookup_table= _beams_angle_table.stamps;

    if(!_pub_laser2d->set_property(_laser_prop)) {
        LLOG(ERROR,"Publish laser property failed");
    }


    SLOG(INFO)<<"Laserscan grab thread started";
    message::cMsgLaser::shared_ptr_t msg(new message::cMsgLaser());
    while (true) {
        if(_stop_request) {
            LLOG(WARNING,"Stop requested");
            break;
        }
        bool ret = _ipcsub_laserscan->read_new();
        if(!ret ) {
            LLOG(ERROR,"Get laserscan2d msg from ipc,Failed");
            continue;
        }
        if(!msg) {
            msg.reset(new message::cMsgLaser);
        }
        if(ipc_msg::to_robsys_msg(_ipcsub_laserscan->data,*msg) != ROBSYS_SUCCESSED) {
            LLOG(ERROR,"Convert ipc-laserscan msg to robsys failed");
            continue;
        }
        _pub_laser2d->swap_publish(msg);
    }
    SLOG(INFO)<<"Laserscan grab thread stopped";

}
void cIpcTrans::tfunc_grab_livox_pts() {
    if(_ipcsub_livox_pts == nullptr) {
        LLOG(ERROR,"Start [livox-pts] grab thread failed,ipc handle is null");
        _error = true;
        return ;
    }
    livox::cCustomMsg::shared_ptr_t msg(new livox::cCustomMsg);
    SLOG(INFO)<<"Livox-pts grab thread started";

    while (true) {
        if(_stop_request) {
            LLOG(WARNING,"Stop requested");
            break;
        }
        bool ret = _ipcsub_livox_pts->read_new();
        if(!ret) {
            LLOG(ERROR,"Get [livox-pts] msg from ipc,Failed");
            continue;
        }
        if(!msg) {
            msg.reset(new livox::cCustomMsg);
        }
        if(ipc_msg::to_robsys_msg(_ipcsub_livox_pts->data,*msg) != ROBSYS_SUCCESSED) {
            LLOG(ERROR,"Convert ipc msg [livox-pts] to robsys failed");
            continue;
        }
        _pub_livox_pts->swap_publish(msg);
    }

    SLOG(INFO)<<"Livox-pts grab thread stopped";
}
void cIpcTrans::tfunc_grab_livox_imu() {
    if(_ipcsub_livox_imu == nullptr) {
        LLOG(ERROR,"Start [livox-imu] grab thread failed,ipc handle is null");
        _error = true;
        return ;
    }
    message::cMsgIMU::shared_ptr_t msg(new message::cMsgIMU);

    SLOG(INFO)<<"Livox-imu grab thread started";

    while (true) {
        if(_stop_request) {
            LLOG(WARNING,"Stop requested");
            break;
        }
        bool ret = _ipcsub_livox_imu->read_new();
        if(!ret) {
            LLOG(ERROR,"Get [livox-imu] msg from ipc,Failed");
            continue;
        }
        if(!msg) {
            msg.reset(new message::cMsgIMU);
        }
        if(ipc_msg::to_robsys_msg(_ipcsub_livox_imu->data,*msg) != ROBSYS_SUCCESSED) {
            LLOG(ERROR,"Convert [livox-imu] msg to robsys failed");
            continue;
        }
        _pub_livox_imu->swap_publish(msg);
    }
    SLOG(INFO)<<"Livox-imu grab thread stopped";
}

}//namespace rob_sys
