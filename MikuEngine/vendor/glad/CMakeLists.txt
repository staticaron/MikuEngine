cmake_minimum_required(VERSION "3.19")

project("glad")

add_library("glad")

target_sources("glad" PRIVATE src/glad.c)
target_include_directories("glad" PRIVATE include
)
