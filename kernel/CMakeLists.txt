cmake_minimum_required(VERSION 3.13...3.22)

project(kernel.elf VERSION 1.0
                   DESCRIPTION "OS Kernel Entrypoint"
                   LANGUAGES ASM_NASM CXX
)

# See https://stackoverflow.com/questions/56420035/how-to-use-cmake-to-build-binaries-with-nasm
# for details regarding some of the settings below. We create a 32-bit ELF
# object library, Loader.
enable_language(ASM_NASM)
set(CAN_USE_ASSEMBLER TRUE)
set(CMAKE_ASM_NASM_OBJECT_FORMAT elf)
set(CMAKE_ASM_NASM_FLAGS "-f elf32")
set(CMAKE_ASM_NASM_COMPILE_OBJECT "<CMAKE_ASM_NASM_COMPILER> <INCLUDES> \
    <FLAGS> -o <OBJECT> <SOURCE>")
    # Create a compile option that operates on ASM_NASM files
    # If the target has a property NASM_OBJ_FORMAT, use it, otherwise
    # use the environment variable CMAKE_ASM_NASM_OBJECT_FORMAT.
    add_compile_options(
        "$<$<COMPILE_LANGUAGE:ASM_NASM>:-f \
         $<IF:$<BOOL:$<TARGET_PROPERTY:NASM_OBJ_FORMAT>>, \
         $<TARGET_PROPERTY:NASM_OBJ_FORMAT>, ${CMAKE_ASM_NASM_OBJECT_FORMAT}>>"
)
add_library(Loader OBJECT loader.nasm)

# CMake uses the compiler as it default linker. We want to use ld to link.
# Set the linker to GNU ld and define the CMake CXX link line.
# See https://stackoverflow.com/questions/1867745/cmake-use-a-custom-linker
# for details.
set(CMAKE_LINKER "/usr/bin/ld")
set(CMAKE_CXX_LINK_EXECUTABLE "<CMAKE_LINKER> <FLAGS> <CMAKE_CXX_LINK_FLAGS> \
    <LINK_FLAGS> <OBJECTS> -o <TARGET> <LINK_LIBRARIES>")

# Creation of the 32-bit kernel ELF.
add_executable(${PROJECT_NAME} kmain.cc)
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

target_link_options(${PROJECT_NAME}
    PRIVATE
        # Make sure ld uses the custom ld script over the default.
        -T ${CMAKE_CURRENT_SOURCE_DIR}/link.ld
        -m elf_i386
)

target_link_libraries(${PROJECT_NAME}
    PRIVATE
        Loader
        Logger
        PortIO
        FrameBuffer
        SerialPort
)

set(KERNEL_INSTALL_DIR "${CMAKE_SOURCE_DIR}/iso/boot")
install(TARGETS ${PROJECT_NAME}
    RUNTIME DESTINATION ${KERNEL_INSTALL_DIR}
)
