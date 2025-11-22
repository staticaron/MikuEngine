cmake_minimum_required(VERSION 3.19)

project(stb_image)

add_library(stb_image)

target_sources(stb_image PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/src/stb_image.cpp")

target_include_directories(stb_image PUBLIC 
	"${CMAKE_CURRENT_SOURCE_DIR}/include/"
)
