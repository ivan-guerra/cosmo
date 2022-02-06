cmake_minimum_required(VERSION 3.13...3.22)

project(libc DESCRIPTION "Freestanding libc Implementation"
             LANGUAGES   C
)

add_library(${PROJECT_NAME} OBJECT
   "${CMAKE_CURRENT_SOURCE_DIR}/stdio/printf.c"
   "${CMAKE_CURRENT_SOURCE_DIR}/stdio/putchar.c"
   "${CMAKE_CURRENT_SOURCE_DIR}/stdio/puts.c"
   "${CMAKE_CURRENT_SOURCE_DIR}/stdlib/abort.c"
   "${CMAKE_CURRENT_SOURCE_DIR}/string/memcmp.c"
   "${CMAKE_CURRENT_SOURCE_DIR}/string/memcpy.c"
   "${CMAKE_CURRENT_SOURCE_DIR}/string/memmove.c"
   "${CMAKE_CURRENT_SOURCE_DIR}/string/memset.c"
   "${CMAKE_CURRENT_SOURCE_DIR}/string/strlen.c"
)

target_include_directories(${PROJECT_NAME}
    PUBLIC
        "${COSMO_INCLUDE_DIR}/libc"
)

target_compile_definitions(${PROJECT_NAME}
    PRIVATE
        -D__is_libc
        -D__is_libk
)

target_compile_options(${PROJECT_NAME}
    PRIVATE
        -Werror
        -ffreestanding
)
