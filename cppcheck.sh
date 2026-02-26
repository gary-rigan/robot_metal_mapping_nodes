#version 0.0.6
cppcheck \
 -I ./ \
 -I /home/liyanx/workspace_env_new/robsys_env/include/ \
 -I /home/liyanx/workspace_env_new//gcc_local/eigen3.4.0/include/eigen3 \
 -I /home/liyanx/workspace_env_new//gcc_local/protobuf3.6.1/include/ \
 -I /home/liyanx/workspace_env_new//gcc_local/opencv3.4.10/include/ \
 --enable=performance --enable=information \
 --enable=all  --inconclusive --force  \
 --verbose .

 #--check-config

