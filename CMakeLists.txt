cmake_minimum_required(VERSION 3.0.0)
project(Islands VERSION 1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

file(GLOB_RECURSE sources CONFIGURE_DEPENDS src/*.cpp src/*.hpp)

file(GLOB_RECURSE sources CONFIGURE_DEPENDS src/*.cpp src/*.hpp)
add_executable(Islands ${sources})
source_group(TREE "${CMAKE_CURRENT_LIST_DIR}" FILES ${sources})

target_include_directories(Islands PUBLIC src/)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)

option(ISLANDS_USE_SDL = OFF)
option(ISLANDS_USE_GLFW = ON)

# OS Macros
if (WIN32)
	target_compile_definitions(Islands PUBLIC ISLANDS_WINDOWS)
endif()

if (${CMAKE_SYSTEM_NAME} STREQUAL "Darwin")
	target_compile_definitions(Islands PUBLIC ISLANDS_OSX)
endif()

if (UNIX AND NOT APPLE)
	target_compile_definitions(Islands PUBLIC ISLANDS_UNIX)
endif()

if (ANDROID)
	target_compile_definitions(Islands PUBLIC ISLANDS_ANDROID)
	if (ISLANDS_USE_GLFW)
		message(FATAL_ERROR "GLFW does not support this operating system")
	endif()
endif()

if (IOS)
	target_compile_definitions(Islands PUBLIC ISLANDS_IOS)
	if (ISLANDS_USE_GLFW)
		message(FATAL_ERROR "GLFW does not support this operating system")
	endif()
endif()

# Filename macro

# Helper function to add preprocesor definition of FILE_BASENAME
# to pass the filename without directory path for debugging use.
#
# Note that in header files this is not consistent with
# __FILE__ and __LINE__ since FILE_BASENAME will be the
# compilation unit source file name (.c/.cpp).
#
# Example:
#
#   define_file_basename_for_sources(my_target)
#
# Will add -DFILE_BASENAME="filename" for each source file depended on
# by my_target, where filename is the name of the file.
#
function(define_file_basename_for_sources targetname)
    get_target_property(source_files "${targetname}" SOURCES)
    foreach(sourcefile ${source_files})
        # Add the FILE_BASENAME=filename compile definition to the list.
        get_filename_component(basename "${sourcefile}" NAME)
        # Set the updated compile definitions on the source file.
        set_property(
            SOURCE "${sourcefile}" APPEND
            PROPERTY COMPILE_DEFINITIONS "FILE_BASENAME=\"${basename}\"")
    endforeach()
endfunction()

define_file_basename_for_sources(Islands)

# Date
set(USE_SYSTEM_TZ_DB ON)
set(BUILD_TZ_LIB ON)
add_subdirectory(dependencies/date)
target_link_libraries(Islands date)
target_link_libraries(Islands date-tz)

# FastNoise
target_include_directories(Islands PUBLIC dependencies/FastNoiseLite/include/)

# Vulkan
find_package(Vulkan REQUIRED FATAL_ERROR)
target_link_libraries(Islands ${Vulkan_LIBRARIES})
target_include_directories(Islands PUBLIC ${Vulkan_INCLUDE_DIR})

# SDL
if (ISLANDS_USE_SDL)
	set(SDL_MAIN_HANDLED TRUE)
	set(SDL_STATIC ON CACHE BOOL "" FORCE)
	set(SDL_SHARED OFF CACHE BOOL "" FORCE)
	add_subdirectory(dependencies/SDL2-2.0.16)
	target_link_libraries(Islands SDL2main SDL2-static)
	target_compile_definitions(Islands PUBLIC ISLANDS_SDL)
endif()

# GLFW
if (ISLANDS_USE_GLFW)
	set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
	set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
	set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
	add_subdirectory(dependencies/glfw-3.3.4)
	target_link_libraries(Islands glfw)
	target_compile_definitions(Islands PUBLIC ISLANDS_GLFW)
endif()

# Copy all resources (assets, shaders, etc)
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
                   COMMAND ${CMAKE_COMMAND} -E copy_directory
                       ${CMAKE_SOURCE_DIR}/assets $<TARGET_FILE_DIR:${PROJECT_NAME}>/.Islands/assets)
