cmake_minimum_required(VERSION 3.13...3.22)

project(GlobalDescriptorTable DESCRIPTION "Global Descriptor Table"
                              LANGUAGES   ASM_NASM CXX
)

add_library(${PROJECT_NAME} OBJECT FlushGDT.nasm)

target_include_directories(${PROJECT_NAME}
    PUBLIC
       "${COSMO_INCLUDE_DIR}/GlobalDescriptorTable"
)

target_compile_options(${PROJECT_NAME}
    PRIVATE
        -Werror
)

target_compile_features(${PROJECT_NAME}
    PRIVATE
        cxx_std_14
)
