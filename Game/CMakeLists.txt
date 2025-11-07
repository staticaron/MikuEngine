cmake_minimum_required(VERSION "3.19")

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

project("Game")

file(GLOB_RECURSE GAME_SOURCES
    "${CMAKE_CURRENT_SOURCE_DIR}/include/*.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp"
)

add_executable("Game" "${GAME_SOURCES}")

target_include_directories("Game" PRIVATE 
    "${CMAKE_CURRENT_SOURCE_DIR}/include"
)

set_target_properties("Game" PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${BIN_DIR}
)