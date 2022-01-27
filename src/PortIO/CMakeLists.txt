cmake_minimum_required(VERSION 3.13...3.22)

project(PortIO DESCRIPTION "Port I/O Routines"
               LANGUAGES ASM_NASM
)

add_library(${PROJECT_NAME} OBJECT PortIO.nasm)

target_include_directories(${PROJECT_NAME}
    PUBLIC
        "${COSMO_INCLUDE_DIR}/PortIO"
)
