cmake_minimum_required(VERSION 3.13...3.22)

project(kernel.elf VERSION 1.0
                   DESCRIPTION "OS Kernel Entrypoint"
                   LANGUAGES   ASM_NASM CXX
)

# Creation of the 32-bit kernel ELF.
add_executable(${PROJECT_NAME} kmain.cc loader.nasm)
target_compile_options(${PROJECT_NAME}
    PRIVATE
        -Werror
)

target_compile_features(${PROJECT_NAME}
    PRIVATE
        cxx_std_14
)

target_link_options(${PROJECT_NAME}
    PRIVATE
        # Make sure ld uses the custom ld script over the default.
        -T ${CMAKE_CURRENT_SOURCE_DIR}/link.ld
        -ffreestanding
        -O2
        -nostdlib
        -o ${PROJECT_NAME}
        -lgcc
)

target_link_libraries(${PROJECT_NAME}
    PRIVATE
        Logger
        PortIO
        FrameBuffer
        SerialPort
        GlobalDescriptorTable
)

set(KERNEL_INSTALL_DIR "${CMAKE_SOURCE_DIR}/iso/boot")
install(TARGETS ${PROJECT_NAME}
    RUNTIME DESTINATION ${KERNEL_INSTALL_DIR}
)
