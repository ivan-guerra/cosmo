cmake_minimum_required(VERSION 3.13...3.22)

project(PortIO DESCRIPTION "Port I/O Routines"
               LANGUAGES ASM_NASM
)

# See https://stackoverflow.com/questions/56420035/how-to-use-cmake-to-build-binaries-with-nasm
# for details regarding some of the settings below. We create a 32-bit ELF
# object library, PortIO.G
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
add_library(${PROJECT_NAME} OBJECT PortIO.nasm)

target_include_directories(${PROJECT_NAME}
    PUBLIC
        "${COSMO_INCLUDE_DIR}/PortIO"
)
