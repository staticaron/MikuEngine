cmake_minimum_required(VERSION "3.19")

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

project("MikuEngine" CXX)

file(GLOB_RECURSE ENGINE_SOURCES
    "${CMAKE_CURRENT_SOURCE_DIR}/include/*.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp"
)

add_library("MikuEngine" SHARED ${ENGINE_SOURCES})

target_include_directories("MikuEngine" PUBLIC
    "${CMAKE_CURRENT_SOURCE_DIR}/include"
)

set_target_properties("MikuEngine" PROPERTIES 
    RUNTIME_OUTPUT_DIRECTORY "${BIN_DIR}" 
)