#include "ipc_trans.h"
#include "internal/rsnode_mapping_config.h"
#include "robsys_parsers/parsers.h"
namespace rob_sys {
namespace {
bool check_config_name(const YAML::Node& node) {
    std::string config_name = node["config_name"].as<std::string>();
    if(node.IsNull()) {
        LLOG(ERROR,"Cannot load yaml filed");
        return false;
    }
    if(config_name != k_rsnode_name) {
        LLOG(ERROR,"Module name [%s] parsersed from config mismatched with current module [%s]"
             ,config_name .c_str(),k_rsnode_name);
        return false;
    }
    return true;
}
}//namespace


int cIpcTrans::load_option_from_yaml(const std::string& filename,option_t& opt) {
    SLOG(INFO)<<"Loading app ["<<k_rsnode_name <<"] option from :" <<filename;


    RS_YAML_TRY
    YAML::Node node = YAML::LoadFile(filename);
    if(!check_config_name(node)) {
        LLOG(ERROR,"Config name check failed");
        return ROBSYS_ERROR;
    }
    {
        YAML::Node ntopic = node["Topics"];
        opt._topic_laserscan  = ntopic["topic_laser"].as<std::string>();
        opt._topic_livox_pts  = ntopic["livox_pts"]  .as<std::string>();
        opt._topic_livox_imu  = ntopic["livox_imu"]  .as<std::string>();
        opt._topic_rgbd_front = ntopic["rgbd_front"] .as<std::string>();

    }
    RS_YAML_CATCH("ipc-opt")

    return ROBSYS_SUCCESSED;
}
void cIpcTrans::show_option(const option_t& opt) {
    std::stringstream ss;

    ss <<"\nApp ["<<k_rsnode_name<<"] option"
       <<"\nTopics:"
       <<"\n>Laserscan2d: "<<opt._topic_laserscan
       <<"\n>Livox-pts:   "<<opt._topic_livox_pts
       <<"\n>Livox-imu:   "<<opt._topic_livox_imu
       <<"\n>Rgbd-front:  "<<opt._topic_livox_imu;
    SLOG(INFO)<<ss.str();

}

}//namespace rob_sys
