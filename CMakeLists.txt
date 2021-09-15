cmake_minimum_required(VERSION 3.0.0)
project(Islands VERSION 1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

file(GLOB_RECURSE sources CONFIGURE_DEPENDS src/*.cpp src/*.hpp)

add_executable(Islands ${sources})

target_include_directories(Islands PUBLIC src/)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)

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
endif()

if (IOS)
	target_compile_definitions(Islands PUBLIC ISLANDS_IOS)
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
add_subdirectory(dependencies/date)
target_link_libraries(Islands date)

# FastNoise
target_include_directories(Islands PUBLIC dependencies/FastNoiseLite/include/)

# SDL
if (WIN32 OR ${CMAKE_SYSTEM_NAME} STREQUAL "Darwin" OR (UNIX AND NOT APPLE) OR ANDROID OR IOS)
	add_subdirectory(dependencies/SDL2-2.0.16)
	set(SDL_MAIN_HANDLED TRUE)
	set(SDL_STATIC ON CACHE BOOL "" FORCE)
	set(SDL_SHARED OFF CACHE BOOL "" FORCE)
	target_link_libraries(Islands SDL2main SDL2-static)
	target_compile_definitions(Islands PUBLIC ISLANDS_SDL)
endif()

# Copy all resources (assets, shaders, etc)
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
                   COMMAND ${CMAKE_COMMAND} -E copy_directory
                       ${CMAKE_SOURCE_DIR}/assets $<TARGET_FILE_DIR:${PROJECT_NAME}>/.Islands/assets)