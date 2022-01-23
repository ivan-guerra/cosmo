cmake_minimum_required(VERSION 3.13...3.22)

project(SerialPort DESCRIPTION "Serial COM Port Driver"
                   LANGUAGES   CXX
)

add_library(${PROJECT_NAME} OBJECT SerialPort.cc)

target_include_directories(${PROJECT_NAME}
    PUBLIC
        "${COSMO_INCLUDE_DIR}/SerialPort"
)

target_compile_options(${PROJECT_NAME}
    PRIVATE
        -m32
        -nostdlib
        -nostdinc
        -fno-builtin
        -fno-stack-protector
        -nostartfiles
        -nodefaultlibs
        -Wall
        -Wextra
        -Werror
        -c
)

target_compile_features(${PROJECT_NAME}
    PRIVATE
        cxx_std_14
)

target_link_libraries(${PROJECT_NAME}
    PRIVATE
        PortIO
)
