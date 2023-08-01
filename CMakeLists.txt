##########################################################
## CMake Setting for SoftRenderer 
##########################################################

cmake_minimum_required(VERSION 3.1)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(CMakeDependentOption)
set(MAIN_PROJECT_NAME SoftRenderer)

set(MODULE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/Source)

project(${MAIN_PROJECT_NAME})

if(NOT WIN32)
	message(FATAL_ERROR "This project only supports windows environment currently.")
endif()

if(NOT MSVC)
	message(FATAL_ERROR "This project only supports MSVC project currently.")
endif()

################################################################################
# Sub projects
################################################################################

add_subdirectory(Source/Math)
add_subdirectory(Source/Renderer)

set(CMAKE_SUPPRESS_REGENERATION true)
set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT Renderer)

