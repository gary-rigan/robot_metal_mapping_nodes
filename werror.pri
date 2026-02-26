
message("---------- Werror flags initialized ------------- V0.0.1")


QMAKE_CFLAGS   +=-Wall
QMAKE_CXXFLAGS +=-Wall
QMAKE_CFLAGS   +=-Wextra
QMAKE_CXXFLAGS +=-Wextra
#QMAKE_CFLAGS   +=-Wpedantic
#QMAKE_CXXFLAGS +=-Wpedantic
#QMAKE_CFLAGS   +=-Werror
#QMAKE_CXXFLAGS +=-Werror


#QMAKE_CFLAGS   +=-Werror=deprecated-copy
#QMAKE_CXXFLAGS +=-Werror=deprecated-copy

QMAKE_CFLAGS   +=-Werror=type-limits
QMAKE_CXXFLAGS +=-Werror=type-limits


QMAKE_CFLAGS   +=-Werror=return-type
QMAKE_CXXFLAGS +=-Werror=return-type

QMAKE_CFLAGS   +=-Werror=reorder
QMAKE_CXXFLAGS +=-Werror=reorder

QMAKE_CFLAGS   +=-Werror=non-virtual-dtor
QMAKE_CXXFLAGS +=-Werror=non-virtual-dtor

QMAKE_CFLAGS   +=-Werror=disabled-optimization
QMAKE_CXXFLAGS +=-Werror=disabled-optimization

QMAKE_CFLAGS   +=-Werror=pointer-arith
QMAKE_CXXFLAGS +=-Werror=pointer-arith

QMAKE_CFLAGS   +=-Werror=uninitialized
QMAKE_CXXFLAGS +=-Werror=uninitialized

QMAKE_CFLAGS   +=-Werror=maybe-uninitialized
QMAKE_CXXFLAGS +=-Werror=maybe-uninitialized

#QMAKE_CFLAGS   +=-Werror=shadow
#QMAKE_CXXFLAGS +=-Werror=shadow



QMAKE_CFLAGS   +=-Werror=return-local-addr
QMAKE_CXXFLAGS +=-Werror=return-local-addr


QMAKE_CFLAGS   +=-Werror=format
QMAKE_CXXFLAGS +=-Werror=format




if(greaterThan(BUILD_GCC_VER,6)) {
    message("Gcc version ig greater than 6,version: " $$BUILD_GCC_VER)
    QMAKE_CFLAGS   +=-Werror=format-truncation
    QMAKE_CXXFLAGS +=-Werror=format-truncation
}

if(greaterThan(BUILD_GCC_VER,7)) {
    message("Gcc version ig greater than 7,version: " $$BUILD_GCC_VER)
    QMAKE_CFLAGS   +=-Werror=class-memaccess
    QMAKE_CXXFLAGS +=-Werror=class-memaccess
    QMAKE_CFLAGS   +=-Werror=implicit-fallthrough
    QMAKE_CXXFLAGS +=-Werror=implicit-fallthrough

}
