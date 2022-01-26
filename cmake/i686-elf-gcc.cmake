cmake_minimum_required(VERSION 3.13...3.22)

# This toolchain was dervied from the examples given in
# https://kubasejdak.com/how-to-cross-compile-for-embedded-with-cmake-like-a-champ

set(CMAKE_SYSTEM_NAME               i686-elf)
set(CMAKE_SYSTEM_PROCESSOR          i686)

# Without that flag CMake is not able to pass test compilation check.
set(CMAKE_TRY_COMPILE_TARGET_TYPE   STATIC_LIBRARY)

# Verify nasm assembler is installed and configure the relevant NASM CMake
# variables.
find_program(NASM nasm)
if (NASM-NOTFOUND)
    message(FATAL_ERROR "nasm assembler missing. Please install nasm.")
endif(NASM-NOTFOUND)
set(CMAKE_ASM_NASM_COMPILER ${NASM})
set(CMAKE_ASM_NASM_OBJECT_FORMAT elf)
set(CMAKE_ASM_NASM_SOURCE_FILE_EXTENSIONS nasm)

set(I686_TOOLCHAIN_PATH "$ENV{HOME}/opt/cross/bin/")
set(CMAKE_AR            ${I686_TOOLCHAIN_PATH}i686-elf-ar${CMAKE_EXECUTABLE_SUFFIX})
set(CMAKE_ASM_COMPILER  ${CMAKE_ASM_NASM_COMPILER})
set(CMAKE_C_COMPILER    ${I686_TOOLCHAIN_PATH}i686-elf-gcc${CMAKE_EXECUTABLE_SUFFIX})
set(CMAKE_CXX_COMPILER  ${I686_TOOLCHAIN_PATH}i686-elf-g++${CMAKE_EXECUTABLE_SUFFIX})
set(CMAKE_LINKER        ${I686_TOOLCHAIN_PATH}i686-elf-ld${CMAKE_EXECUTABLE_SUFFIX})
set(CMAKE_OBJCOPY       ${I686_TOOLCHAIN_PATH}i686-elf-objcopy${CMAKE_EXECUTABLE_SUFFIX} CACHE INTERNAL "")
set(CMAKE_RANLIB        ${I686_TOOLCHAIN_PATH}i686-elf-ranlib${CMAKE_EXECUTABLE_SUFFIX} CACHE INTERNAL "")
set(CMAKE_SIZE          ${I686_TOOLCHAIN_PATH}i686-elf-size${CMAKE_EXECUTABLE_SUFFIX} CACHE INTERNAL "")
set(CMAKE_STRIP         ${I686_TOOLCHAIN_PATH}i686-elf-strip${CMAKE_EXECUTABLE_SUFFIX} CACHE INTERNAL "")

# These default C/C++ flags were lifted directly from the OSDev wiki:
# https://wiki.osdev.org/Bare_Bones#Implementing_the_Kernel
set(CMAKE_C_FLAGS   "-ffreestanding -O2 -Wall -Wextra" CACHE INTERNAL "")
set(CMAKE_CXX_FLAGS "${CMAKE_C_FLAGS} -ffreestanding -O2 -Wall -Wextra -fno-exceptions -fno-rtti" CACHE INTERNAL "")

set(CMAKE_C_FLAGS_DEBUG     "-Os -g" CACHE INTERNAL "")
set(CMAKE_C_FLAGS_RELEASE   "-Os -DNDEBUG" CACHE INTERNAL "")
set(CMAKE_CXX_FLAGS_DEBUG   "${CMAKE_C_FLAGS_DEBUG}" CACHE INTERNAL "")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE}" CACHE INTERNAL "")

set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
