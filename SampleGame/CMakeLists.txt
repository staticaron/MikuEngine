cmake_minimum_required(VERSION "3.19")

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

project("SampleGameLogic" LANGUAGES CXX)

file(GLOB_RECURSE GAME_LOGIC_SOURCES
    "${CMAKE_CURRENT_SOURCE_DIR}/assets/*.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/assets/*.cpp"
)

add_library("SampleGameLogic" SHARED ${GAME_LOGIC_SOURCES})

target_include_directories("SampleGameLogic" PUBLIC
    "${CMAKE_CURRENT_SOURCE_DIR}/assets/"
)

target_link_libraries("SampleGameLogic" PRIVATE "MikuEngine")

set_target_properties("SampleGameLogic" PROPERTIES 
	RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
	LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)

