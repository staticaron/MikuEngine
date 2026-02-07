cmake_minimum_required(VERSION "3.19")

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_COLOR_DIAGNOSTICS ON)

project("RhythmGame")

set(BIN_DIR "${CMAKE_BINARY_DIR}/bin")

add_subdirectory(MikuEngine)
add_subdirectory(MikuEditor)
add_subdirectory(Game)
