TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
        internal/stop_request.cpp \
        main.cpp


message("==============================Project example [$$TARGET]============================")

include(../../robsys_build_config/qt_env.pri)
include($${RSWS_BUIILD_ENV}/qmake/robsys_platform.pri)
include($${RSWS_BUIILD_ENV}/qmake/robsys_rpath.pri)

include($${RSWS_BUIILD_ENV}/modules/RobsysFindEigen.pri)
#include($${RSWS_BUIILD_ENV}/modules/RobsysFindOpencv3.pri)
include($${RSWS_BUIILD_ENV}/modules/RobsysFindProtobuf3.6.1.pri)

INCLUDEPATH += $${ROBSYS_ENV_PATH}/include/
INCLUDEPATH += $${ROBSYS_ENV_PATH}/include/utils
INCLUDEPATH += $${ROBSYS_ENV_PATH}/include/parsers
INCLUDEPATH += $${ROBSYS_ENV_PATH}/include/robsys_system
INCLUDEPATH += $${ROBSYS_ENV_PATH}/include/robsys_message

INCLUDEPATH += $${ROBSYS_EIGEN3_INCLUDE}



LIB_THR_PARTY_PATH =$${ROBSYS_PLATFORM_ENV_PATH}/lib/
LIB_ROBSYS_PATH    =$${ROBSYS_PLATFORM_ENV_PATH}/yogo_lib/
BIN_INSTALL_PATH   =$${ROBSYS_PLATFORM_ENV_PATH}/node_bin/

message("Thr party  lib path: " $${LIB_THR_PARTY_PATH})
message("Robsys     lib path: " $${LIB_ROBSYS_PATH})
message("Target install path: " $${BIN_INSTALL_PATH})

LIBS += -L$${LIB_ROBSYS_PATH}/ -lrobsys_system -ltransform_server
LIBS += -L$${LIB_ROBSYS_PATH}/ -lutils -lrobsys_message -lparsers -lrobsys_datasets

LIBS += -L$${ROBSYS_PROTOBUF_LIBRARIES}


LIBS += -lpthread -ldl -lrt



target.path = $${BIN_INSTALL_PATH}
!isEmpty(target.path): INSTALLS += target

HEADERS += \
    internal/ipc_trans.h \
    internal/stop_request.h
