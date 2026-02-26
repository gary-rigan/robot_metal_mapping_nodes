
echo "Compile start ..." 
conan create . navi/develop -pr:h=armv7-dr        -pr:b=default
ret=$?
if [ $ret -eq 1 ];then
    echo "Build project failed [armv7]"
    exit ret;
fi

conan create . navi/develop -pr:h=armv8-dr        -pr:b=default
ret=$?
if [ $ret -eq 1 ];then
    echo "Build project failed [armv8]"
    exit ret;
fi



conan create . navi/develop -pr:h=arm3588-dr        -pr:b=default
ret=$?
if [ $ret -eq 1 ];then
    echo "Build project failed [rk3588]"
    exit ret;
fi

conan create . navi/develop -pr:h=arm3588-dr_gcc7.5 -pr:b=default
ret=$?
if [ $ret -eq 1 ];then
    echo "Build project failed [rk3588-gcc7.5]"
    exit ret;
fi

conan create . navi/develop -pr:h=arm3588-dr_gcc8.3 -pr:b=default
ret=$?
if [ $ret -eq 1 ];then
    echo "Build project failed [rk3588-gcc8.3]"
    exit ret;
fi


conan create . navi/develop -pr:h=x86-dr_opengl     -pr:b=default
ret=$?
if [ $ret -eq 1 ];then
    echo "Build project failed [x86-dr_opengl]"
    exit ret;
fi

