cmake_minimum_required(VERSION 3.0.2)
 
set(CMAKE_CONFIGURATION_TYPES "Debug;Release;RelWithDebInfo")

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/CMake;${CMAKE_MODULE_PATH}")
include(cotire) 

project(OG)

add_subdirectory(${CMAKE_SOURCE_DIR}/core)
add_subdirectory(${CMAKE_SOURCE_DIR}/og-net)
add_subdirectory(${CMAKE_SOURCE_DIR}/test-core)
add_subdirectory(${CMAKE_SOURCE_DIR}/util/db-updater)
add_subdirectory(${CMAKE_SOURCE_DIR}/test-og-net)

