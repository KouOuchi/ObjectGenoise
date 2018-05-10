cmake_minimum_required(VERSION 3.0.2)
 
#
# Options
#
set(SOCI_SOURCE_DIR "E:/git/ObjectGenoise.Resources/soci-3.2.3" CACHE PATH "")
set(SOCI_BUILD_DIR "E:/git/ObjectGenoise.Resources/soci-3.2.3/build" CACHE PATH "")
set(SOCI_CORE_LIBRARY optimized ${SOCI_BUILD_DIR}/lib/Release/libsoci_core_3_2.lib debug ${SOCI_BUILD_DIR}/lib/Debug/libsoci_core_3_2.lib)
set(SOCI_SQLITE3_LIBRARY optimized ${SOCI_BUILD_DIR}/lib/Release/libsoci_sqlite3_3_2.lib debug ${SOCI_BUILD_DIR}/lib/Debug/libsoci_sqlite3_3_2.lib)
set(SQLITE3_SOURCE_DIR "E:/git/ObjectGenoise.Resources/sqlite-amalgamation-3220000/src" CACHE PATH "")
set(SQLITE3_BUILD_DIR "E:/git/ObjectGenoise.Resources/sqlite-amalgamation-3220000/build" CACHE PATH "")
set(SQLITE3_LIBRARY optimized ${SQLITE3_BUILD_DIR}/Release/sqlite3.lib debug ${SQLITE3_BUILD_DIR}/Debug/sqlite3.lib)
set(BOOST_ROOT $ENV{BOOST} CACHE PATH "" )
set(Boost_LIBRARY_DIR ${BOOST_ROOT}/lib64-msvc-14.0 CACHE PATH "" )

set(CMAKE_CONFIGURATION_TYPES "Debug;Release;RelWithDebInfo")

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/CMake;${CMAKE_MODULE_PATH}")
include(cotire) 

project(OG)

add_subdirectory(${CMAKE_SOURCE_DIR}/core)
add_subdirectory(${CMAKE_SOURCE_DIR}/og-net)
add_subdirectory(${CMAKE_SOURCE_DIR}/test-core)
add_subdirectory(${CMAKE_SOURCE_DIR}/util/db-updater)
add_subdirectory(${CMAKE_SOURCE_DIR}/test-og-net)

