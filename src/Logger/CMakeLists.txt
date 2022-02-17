cmake_minimum_required(VERSION 3.13...3.22)

project(Logger DESCRIPTION "Data Logging Utility"
               LANGUAGES   CXX
)

add_library(${PROJECT_NAME} OBJECT Logger.cc)

target_include_directories(${PROJECT_NAME}
    PUBLIC
        "${COSMO_INCLUDE_DIR}/Logger"
)

target_compile_options(${PROJECT_NAME}
    PRIVATE
        -Werror
    INTERFACE
        # Adding this to remove warning issued when logging fmt strings with
        # no arguments.
        -Wno-unused-value
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
        libc
)
