project(json VERSION 2.1.1 LANGUAGES CXX)

add_library(json INTERFACE)

target_include_directories(json INTERFACE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include/>)
