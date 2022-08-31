project(og-net)

# Specify build paths
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")

#
# Find Boost
#

IF (UNIX)
   add_definitions(-DBOOST_ALL_DYN_LINK -DBOOST_LOG_DYN_LINK)
   SET( CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -fPIC -fpermissive -std=c++17")
   link_directories($Boost_Library_Dir)

   #set(Boost_USE_STATIC_LIBS        ON)
   #set(Boost_USE_MULTITHREADED      ON)
   #set(Boost_USE_STATIC_RUNTIME    ON)
   #find_package( Boost REQUIRED )
   
ELSE ()
   include(GenerateExportHeader)
   # Activate C++ exception handling
   IF (NOT CMAKE_CXX_FLAGS MATCHES "/MP")
       SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /MP") 
   ENDIF ()
   IF (NOT CMAKE_CXX_FLAGS MATCHES "-Zm")
       SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Zm250") 
   ENDIF ()
   IF (NOT CMAKE_CXX_FLAGS MATCHES "/GS")
       SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /GS") 
   ENDIF ()
   IF (NOT CMAKE_CXX_FLAGS MATCHES "/Zc:wchar_t")
       SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /Zc:wchar_t") 
   ENDIF ()
   IF (CMAKE_CXX_FLAGS_DEBUG MATCHES "/RTC1")
       string(REPLACE "/RTC1" " " CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG}")
   ENDIF()
   IF(CMAKE_CXX_FLAGS MATCHES "/EHsc")
       string(REPLACE "/EHsc" "" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
   ENDIF()

   SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /MD") 
   SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /MDd") 

#   set(Boost_USE_STATIC_LIBS        ON)
#   set(Boost_USE_MULTITHREADED      ON)
#   set(Boost_USE_STATIC_RUNTIME     OFF)
#   find_package( Boost REQUIRED COMPONENTS iostreams log log_setup zlib)

ENDIF (UNIX)

include_directories(
    ../core/include
    include
    ${Boost_INCLUDE_DIR}
)

link_directories(
    ${Boost_LIBRARY_DIR}
)

FILE(GLOB_RECURSE HDRS ${CMAKE_CURRENT_SOURCE_DIR}/include/*.h)
FILE(GLOB_RECURSE SRCS ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp)

IF (UNIX)
    add_library(og-net ${HDRS} ${SRCS})
ELSE ()
    add_library(og-net SHARED ${HDRS} ${SRCS})

    # static libs
    target_link_libraries(og-net core)
    set(CMAKE_EXE_LINKER_FLAGS
      "${CMAKE_EXE_LINKER_FLAGS} /NODEFAULTLIB:LIBCMT")

ENDIF (UNIX)

#set_target_properties(og-net PROPERTIES DEBUG_POSTFIX d)

add_definitions(-DBOOST_ALL_NO_LIB)
#add_definitions(-D_SCL_SECURE_NO_WARNINGS)
#add_definitions(-D_CRT_SECURE_NO_WARNINGS)
add_definitions(-D_UNICODE)
add_definitions(-DUNICODE)
add_definitions(-D_DLL)
add_definitions(-DBOOST_MPL_CFG_NO_PREPROCESSED_HEADERS)
add_definitions(-DBOOST_MPL_LIMIT_LIST_SIZE=30)
remove_definitions(-D_MBCS)

add_dependencies(og-net core)


#
# cotire
#
add_definitions(-DCOTIRE_MINIMUM_NUMBER_OF_TARGET_SOURCES=1)
set_directory_properties(PROPERTIES
    COTIRE_PREFIX_HEADER_IGNORE_PATH 
    "${PROJECT_SOURCE_DIR}"
    )
set_target_properties(og-net PROPERTIES COTIRE_ADD_UNITY_BUILD FALSE)

set_target_properties(og-net PROPERTIES COMPILE_FLAGS "/clr /EHa")
set_property(TARGET og-net PROPERTY OUTPUT_NAME "og.net")

#
# install
#
set(CMAKE_INSTALL_PREFIX "${CMAKE_BINARY_DIR}/SDK")
install (TARGETS ${PROJECT_NAME} CONFIGURATIONS Release
    DESTINATION "${CMAKE_BINARY_DIR}/SDK/Release/bin")
install (TARGETS ${PROJECT_NAME} CONFIGURATIONS Debug
    DESTINATION "${CMAKE_BINARY_DIR}/SDK/Debug/bin")
install (TARGETS ${PROJECT_NAME} CONFIGURATIONS RelWithDebInfo
    DESTINATION "${CMAKE_BINARY_DIR}/SDK/RelWithDebInfo/bin")


