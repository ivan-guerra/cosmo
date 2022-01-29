cmake_minimum_required(VERSION 3.13...3.22)

project(InterruptDescriptorTable DESCRIPTION "Interrupt Descriptor Table"
                                 LANGUAGES   ASM_NASM CXX
)

add_library(${PROJECT_NAME}
    OBJECT
        InterruptHandler.cc
        InterruptHandler.nasm
        FlushIDT.nasm
        InterruptDescriptorTable.cc
)

target_include_directories(${PROJECT_NAME}
    PUBLIC
       "${COSMO_INCLUDE_DIR}/InterruptDescriptorTable"
)

target_compile_options(${PROJECT_NAME}
    PRIVATE
        -Werror
)

target_compile_features(${PROJECT_NAME}
    PRIVATE
        cxx_std_14
)

target_link_libraries(${PROJECT_NAME}
    PRIVATE
        PortIO
        FrameBuffer
        SerialPort
        Logger
)
