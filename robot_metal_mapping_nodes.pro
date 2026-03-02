TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
        internal/ipc_trans.cpp \
        internal/ipc_trans_option.cpp \
        internal/module_list.cpp \
        internal/robot_modules.cpp \
        internal/stop_request.cpp \
        main.cpp


HEADERS += \
    internal/ipc_trans.h \
    internal/module_list.h \
    internal/robot_modules.h \
    internal/rsnode_mapping_config.h \
    internal/stop_request.h

DISTFILES += \
    conanfile.py \
    config/rsnode_mapping_ipc.yaml \
    config/rsnode_mapping_modules.yaml \
    version.txt


message("==============================Project example [$$TARGET]============================")
DEFINES += "ROBSYS_LOG_TAG=App"
!equals(USE_CONAN, 1)  {
    BUILD_GCC_VER=8
    TARGET_ARCH=x86_64
}
include($$PWD/werror.pri)

equals(USE_CONAN, 1) {

    CONFIG += conan_basic_setup
    QMAKE_RPATHDIR += ./
    QMAKE_RPATHDIR += ../lib
    QMAKE_RPATHDIR += ../libs
    message("use conan dependency")
    include($${OUT_PWD}/conanbuildinfo.pri)
    DEFINES+=BUILD_BY_CONAN

    LIBS += -lpthread -ldl -lrt

}
else {

include(../../robsys_build_config/qt_env.pri)
include($${RSWS_BUIILD_ENV}/qmake/robsys_platform.pri)
include($${RSWS_BUIILD_ENV}/qmake/robsys_rpath.pri)

include($${RSWS_BUIILD_ENV}/modules/RobsysFindEigen.pri)
#include($${RSWS_BUIILD_ENV}/modules/RobsysFindOpencv3.pri)
include($${RSWS_BUIILD_ENV}/modules/RobsysFindProtobuf3.6.1.pri)

INCLUDEPATH += $${ROBSYS_ENV_PATH}/include/
INCLUDEPATH += $${ROBSYS_ENV_PATH}/include/utils
INCLUDEPATH += $${ROBSYS_ENV_PATH}/include/robsys_parsers
INCLUDEPATH += $${ROBSYS_ENV_PATH}/include/robsys_system
INCLUDEPATH += $${ROBSYS_ENV_PATH}/include/robsys_message

INCLUDEPATH += $${ROBSYS_EIGEN3_INCLUDE}



LIB_THR_PARTY_PATH =$${ROBSYS_PLATFORM_ENV_PATH}/lib/
LIB_ROBSYS_PATH    =$${ROBSYS_PLATFORM_ENV_PATH}/yogo_lib/
BIN_INSTALL_PATH   =$${ROBSYS_PLATFORM_ENV_PATH}/node_bin/

message("Thr party  lib path: " $${LIB_THR_PARTY_PATH})
message("Robsys     lib path: " $${LIB_ROBSYS_PATH})
message("Target install path: " $${BIN_INSTALL_PATH})



LIBS += -L$${LIB_ROBSYS_PATH}/ -lrobot_metal_ipc_msg -lrobot_metal_ipc_msg_bridge -ldevice_laserscan_livox_message
LIBS += -L$${LIB_ROBSYS_PATH}/ -lrobsys_system -ltransform_server
LIBS += -L$${LIB_ROBSYS_PATH}/ -lutils -lrobsys_message -lrobsys_parsers -lrobsys_datasets

LIBS += -L$${ROBSYS_PROTOBUF_LIBRARIES}


LIBS += -lpthread -ldl -lrt



target.path = $${BIN_INSTALL_PATH}
!isEmpty(target.path): INSTALLS += target

}
