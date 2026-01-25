cmake_minimum_required(VERSION "3.19")

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

project("MikuEngine" LANGUAGES CXX)

add_subdirectory(vendor/glad)
add_subdirectory(vendor/glfw)
add_subdirectory(vendor/spdlog)
add_subdirectory(vendor/imgui)
add_subdirectory(vendor/glm)
add_subdirectory(vendor/stb_image)
add_subdirectory(vendor/entt)
add_subdirectory(vendor/yaml-cpp)
add_subdirectory(vendor/nativefiledialog)
add_subdirectory(vendor/imguizmo)

file(GLOB_RECURSE ENGINE_SOURCES
    "${CMAKE_CURRENT_SOURCE_DIR}/include/*.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp"
)

add_library("MikuEngine" SHARED ${ENGINE_SOURCES})

target_include_directories("MikuEngine" PUBLIC
    "${CMAKE_CURRENT_SOURCE_DIR}/include"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/MikuEngine"
	"${CMAKE_CURRENT_SOURCE_DIR}/vendor/glad/include"
	"${CMAKE_CURRENT_SOURCE_DIR}/vendor/stb_image/include"
)

target_link_libraries("MikuEngine" PUBLIC 
	glad
	glfw
	spdlog
	imgui
	imguizmo
	glm
	stb_image
	entt
	yaml-cpp
	nfd
)

target_link_libraries("MikuEngine" PRIVATE 
	X11
    Xi
    Xrandr
    Xcursor
    Xinerama
)

target_compile_definitions("MikuEngine" PUBLIC
	RESOURCE_DIR="${CMAKE_CURRENT_SOURCE_DIR}/assets/"
	PROJECT_DIR="${CMAKE_SOURCE_DIR}/SampleProject/assets/"
)

target_compile_definitions("MikuEngine" PUBLIC
	$<$<CONFIG:Debug>:MIKU_DEBUG>
	$<$<CONFIG:Release>:MIKU_RELEASE>
)

set_target_properties("MikuEngine" PROPERTIES 
	LIBRARY_OUTPUT_DIRECTORY "${BIN_DIR}" 
)
