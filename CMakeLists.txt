cmake_minimum_required(VERSION 3.13...3.22)

# Require out-of-source builds.
file(TO_CMAKE_PATH "${PROJECT_BINARY_DIR}/CMakeLists.txt" LOC_PATH)
if(EXISTS "${LOC_PATH}")
    message(FATAL_ERROR "You cannot build in a source directory (or any directory with a CMakeLists.txt file). Please make a build subdirectory. Feel free to remove CMakeCache.txt and CMakeFiles.")
endif()

project(cosmo VERSION 1.0
              DESCRIPTION "OS Development Research Project"
)

set(COSMO_INCLUDE_DIR "${CMAKE_SOURCE_DIR}/include" CACHE STRING "Description")

add_subdirectory(docs)
add_subdirectory(src)
add_subdirectory(kernel)
