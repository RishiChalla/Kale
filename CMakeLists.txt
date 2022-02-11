cmake_minimum_required(VERSION 3.0.0)
project(Kale VERSION 1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

file(GLOB_RECURSE sources CONFIGURE_DEPENDS src/*.cpp src/*.hpp)
add_library(Kale STATIC ${sources})
source_group(TREE "${CMAKE_CURRENT_LIST_DIR}" FILES ${sources})

target_include_directories(Kale PUBLIC src/)
target_include_directories(Kale INTERFACE src/)

set(CMAKE_POLICY_DEFAULT_CMP0077 NEW)

option(KALE_USE_SDL = OFF)
option(KALE_USE_GLFW = ON)
option(KALE_VERBOSE = OFF)

# OS Macros
if (WIN32)
	target_compile_definitions(Kale PUBLIC KALE_WINDOWS)
	target_compile_definitions(Kale PUBLIC NOMINMAX)
endif()

if (${CMAKE_SYSTEM_NAME} STREQUAL "Darwin")
	target_compile_definitions(Kale PUBLIC KALE_OSX)
endif()

if (UNIX AND NOT APPLE)
	target_compile_definitions(Kale PUBLIC KALE_UNIX)
endif()

if (ANDROID)
	target_compile_definitions(Kale PUBLIC KALE_ANDROID)
	if (KALE_USE_GLFW)
		message(FATAL_ERROR "GLFW does not support this operating system")
	endif()
endif()

if (IOS)
	target_compile_definitions(Kale PUBLIC KALE_IOS)
	if (KALES_USE_GLFW)
		message(FATAL_ERROR "GLFW does not support this operating system")
	endif()
endif()

# Debug Macros
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
	target_compile_definitions(Kale PUBLIC KALE_DEBUG)
	set(KALE_DEBUG ON)
endif()

if (KALE_VERBOSE)
	target_compile_definitions(Kale PUBLIC KALE_VERBOSE)
endif()

# Date
set(USE_SYSTEM_TZ_DB ON)
set(BUILD_TZ_LIB ON)
add_subdirectory(dependencies/date)
target_link_libraries(Kale date)
target_link_libraries(Kale date-tz)

# FastNoise
target_include_directories(Kale PUBLIC dependencies/FastNoiseLite/Cpp/)

# Json
set(JSON_BuildTests OFF CACHE INTERNAL "")
add_subdirectory(dependencies/json)
target_link_libraries(Kale nlohmann_json)

# Skia

# To generate skia files (m1 mac), change debug/official build depending on debug or not -
# bin/gn gen out/Static/Release --args='is_debug=false is_official_build=true skia_use_gl=true skia_use_system_expat=false skia_use_system_harfbuzz=false
# skia_use_system_icu=false skia_use_system_libjpeg_turbo=false skia_use_system_libpng=false skia_use_system_libwebp=false skia_use_system_zlib=false
# target_cpu="arm64"'

set(SKIA_DIR "${CMAKE_CURRENT_SOURCE_DIR}/dependencies/skia")

target_include_directories(Kale PUBLIC
	${SKIA_DIR}/
	${SKIA_DIR}/include
)

target_compile_definitions(Kale PUBLIC SK_GL)

if (WIN32)
	if(KALE_DEBUG)
		target_link_libraries(Kale ${SKIA_DIR}/out/Static/Debug/libskia.lib)
		target_link_libraries(Kale ${SKIA_DIR}/out/Static/Debug/libpathkit.lib)
	else()
		target_link_libraries(Kale ${SKIA_DIR}/out/Static/Release/libskia.lib)
		target_link_libraries(Kale ${SKIA_DIR}/out/Static/Release/libpathkit.lib)
	endif()
endif()
if (UNIX)
	if(KALE_DEBUG)
		target_link_libraries(Kale ${SKIA_DIR}/out/Static/Debug/libskia.a)
		target_link_libraries(Kale ${SKIA_DIR}/out/Static/Debug/libpathkit.a)
	else()
		target_link_libraries(Kale ${SKIA_DIR}/out/Static/Release/libskia.a)
		target_link_libraries(Kale ${SKIA_DIR}/out/Static/Release/libpathkit.a)
	endif()
endif()

if (APPLE)
    target_link_libraries(Kale "-framework CoreServices")
    target_link_libraries(Kale "-framework CoreGraphics")
    target_link_libraries(Kale "-framework CoreText")
    target_link_libraries(Kale "-framework CoreFoundation")
endif()

# Termcolor - Debug only since console is not used for release
if(KALE_DEBUG)
	add_subdirectory(dependencies/termcolor)
	target_link_libraries(Kale termcolor)
endif()

# SDL
if (KALE_USE_SDL)
	set(SDL_MAIN_HANDLED TRUE)
	set(SDL_STATIC ON CACHE BOOL "" FORCE)
	set(SDL_SHARED OFF CACHE BOOL "" FORCE)
	add_subdirectory(dependencies/SDL2)
	target_link_libraries(Kale SDL2main SDL2-static)
	target_compile_definitions(Kale PUBLIC KALE_SDL)
endif()

# GLFW
if (KALE_USE_GLFW)
	set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
	set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
	set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
	add_subdirectory(dependencies/glfw)
	target_link_libraries(Kale glfw)
	target_compile_definitions(Kale PUBLIC KALE_GLFW)
endif()

# Shaders
set(SHADER_BINARY_DIR ${CMAKE_CURRENT_SOURCE_DIR}/assets/shaders/)

# Copy Skia Shaders
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
	COMMAND ${CMAKE_COMMAND} -E copy_directory
	${CMAKE_CURRENT_SOURCE_DIR}/shaders/ ${SHADER_BINARY_DIR})
