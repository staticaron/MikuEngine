cmake_minimum_required(VERSION "3.19")

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

project("MikuEditor")

file(GLOB_RECURSE GAME_SOURCES
    "${CMAKE_CURRENT_SOURCE_DIR}/include/*.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp"
)

add_executable("MikuEditor" "${GAME_SOURCES}")

target_include_directories("MikuEditor" PRIVATE 
    "${CMAKE_CURRENT_SOURCE_DIR}/include"
)

target_link_libraries("MikuEditor" PRIVATE 
	MikuEngine
)
set_target_properties("MikuEditor" PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${BIN_DIR}
)

set_target_properties("MikuEditor" PROPERTIES 
	ENABLE_EXPORTS ON
)

# 2. Linux specific: force all symbols into the dynamic symbol table
if(UNIX AND NOT APPLE)
    target_link_options("MikuEditor" PUBLIC "-rdynamic")
endif()
