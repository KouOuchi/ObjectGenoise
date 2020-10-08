project(test-core)

# Specify build paths
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")

IF (UNIX)
   add_definitions(-DBOOST_ALL_DYN_LINK -DBOOST_LOG_DYN_LINK)
   SET( CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -fPIC -fpermissive -std=c++0x")
   link_directories($ENV{BOOST}/stage/x64/lib)

   #set(Boost_USE_STATIC_LIBS        ON)
   #set(Boost_USE_MULTITHREADED      ON)
   #set(Boost_USE_STATIC_RUNTIME    ON)
   #find_package( Boost REQUIRED )
   
ELSE ()
   include(GenerateExportHeader)
   # Activate C++ exception handling
   IF (NOT CMAKE_CXX_FLAGS MATCHES "/EHsc")
       SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /EHsc") 
   ENDIF ()
   IF (NOT CMAKE_CXX_FLAGS MATCHES "/MP")
       SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /MP") 
   ENDIF ()
   IF (NOT CMAKE_CXX_FLAGS MATCHES "-Zm")
       SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Zm250") 
   ENDIF ()
   IF (NOT CMAKE_CXX_FLAGS MATCHES "/GL")
       SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /GL") 
   ENDIF ()
   IF (NOT CMAKE_CXX_FLAGS MATCHES "/Gy")
       SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /Gy") 
   ENDIF ()

   SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /MD") 
   SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /MDd") 

   set(Boost_USE_STATIC_LIBS        ON)
   set(Boost_USE_MULTITHREADED      ON)
   set(Boost_USE_STATIC_RUNTIME     OFF)
   find_package( Boost REQUIRED COMPONENTS iostreams log log_setup unit_test_framework zlib)

ENDIF (UNIX)

include_directories(
	../core/include
    include
    ${SOCI_SOURCE_DIR}/include
    ${SOCI_BUILD_DIR}/include
	${Boost_INCLUDE_DIR}
    ${SQLITE3_SOURCE_DIR}
)

link_directories(
    ${Boost_LIBRARY_DIR}
)

FILE(GLOB_RECURSE HDRS ${CMAKE_CURRENT_SOURCE_DIR}/include/*.h)
FILE(GLOB_RECURSE SRCS ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp)

IF (UNIX)
    add_executable(test-core ${HDRS} ${SRCS})
ELSE ()
    add_executable(test-core ${HDRS} ${SRCS})

    set(MyBoost_THREAD_LIBRARY optimized "boost_thread-vc140-mt-1_63.lib" debug "boost_thread-vc140-mt-gd-1_63.lib")
    set(MyBoost_DATE_TIME_LIBRARY optimized "boost_date_time-vc140-mt-1_63.lib" debug "boost_date_time-vc140-mt-gd-1_63.lib")
    set(MyBoost_SYSTEM_LIBRARY optimized "boost_system-vc140-mt-1_63.lib" debug "boost_system-vc140-mt-gd-1_63.lib")
    set(MyBoost_FILESYSTEM_LIBRARY optimized "boost_filesystem-vc140-mt-1_63.lib" debug "boost_filesystem-vc140-mt-gd-1_63.lib")
    set(MyBoost_CHRONO_LIBRARY optimized "boost_chrono-vc140-mt-1_63.lib" debug "boost_chrono-vc140-mt-gd-1_63.lib")
    set(MyBoost_TIMER_LIBRARY optimized "boost_timer-vc140-mt-1_63.lib" debug "boost_timer-vc140-mt-gd-1_63.lib")
    set(MyBoost_REGEX_LIBRARY optimized "boost_regex-vc140-mt-1_63.lib" debug "boost_regex-vc140-mt-gd-1_63.lib")

    # static libs
    target_link_libraries(test-core core ${SQLITE3_LIBRARY} ${SOCI_CORE_LIBRARY} ${SOCI_SQLITE3_LIBRARY} ${Boost_ZLIB_LIBRARY} ${Boost_IOSTREAMS_LIBRARY} ${Boost_LOG_LIBRARY} ${Boost_LOG_SETUP_LIBRARY} ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY}  ${MyBoost_THREAD_LIBRARY} ${MyBoost_DATE_TIME_LIBRARY} ${MyBoost_SYSTEM_LIBRARY} ${MyBoost_FILESYSTEM_LIBRARY} ${MyBoost_CHRONO_LIBRARY} ${MyBoost_TIMER_LIBRARY} ${MyBoost_REGEX_LIBRARY})

    set(CMAKE_EXE_LINKER_FLAGS
      "${CMAKE_EXE_LINKER_FLAGS} /NODEFAULTLIB:LIBCMT")

ENDIF (UNIX)

set_target_properties(test-core PROPERTIES DEBUG_POSTFIX d)

add_definitions(-DBOOST_ALL_NO_LIB)
#add_definitions(-D_SCL_SECURE_NO_WARNINGS)
#add_definitions(-D_CRT_SECURE_NO_WARNINGS)
add_definitions(-D_UNICODE)
add_definitions(-DUNICODE)
add_definitions(-DBOOST_MPL_CFG_NO_PREPROCESSED_HEADERS)
add_definitions(-DBOOST_MPL_LIMIT_LIST_SIZE=30)
remove_definitions(-D_MBCS)

#
# cotire
#
#add_definitions(-DCOTIRE_MINIMUM_NUMBER_OF_TARGET_SOURCES=1)
#set_directory_properties(PROPERTIES
#    COTIRE_PREFIX_HEADER_IGNORE_PATH 
#    "${PROJECT_SOURCE_DIR}"
#    )
#set_target_properties(test-core PROPERTIES COTIRE_ADD_UNITY_BUILD FALSE)

add_dependencies(test-core core)
