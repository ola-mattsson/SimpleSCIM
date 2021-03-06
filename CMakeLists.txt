cmake_minimum_required(VERSION 3.10)
project(EgilSCIMClient CXX)

set(GroupSCIM_VERSION_MAJOR 1)
set(GroupSCIM_VERSION_MINOR 6)

configure_file(
        "${PROJECT_SOURCE_DIR}/src/utility/GroupSCIM_config.h.in"
        "${PROJECT_BINARY_DIR}/GroupSCIM_config.h"
)

include_directories("${PROJECT_BINARY_DIR}")

set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wshadow -fexceptions -fsanitize=address")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -fsanitize-address-use-after-scope -Wshadow-uncaptured-local")
if (APPLE)
    #all ldap deprecated on apple platforms
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-deprecated-declarations")
endif ()

set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -DNDEBUG -Wall ")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17")

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/Modules")

find_package(CURL REQUIRED)
if (NOT CURL_FOUND)
    message(FATAL_ERROR "please install curl")
else()
    include_directories(${CURL_INCLUDE_DIRS})
endif ()

find_package(Boost REQUIRED)
if (NOT Boost_FOUND)
    message(FATAL_ERROR "please install boost")
else ()
    include_directories(${Boost_INCLUDE_DIRS})
endif ()

set(LDFLAGS -lldap -llber -lcurl -lsqlite3)

link_libraries(${LDFLAGS})

set(srcroot src)
set(SOURCES ${srcroot}/scim_json_parse.cpp ${srcroot}/utility/simplescim_error_string.cpp ${srcroot}/simplescim_scim_send.cpp ${srcroot}/cache_file.cpp ${srcroot}/model/base_object.cpp ${srcroot}/simplescim_ldap_attrs_parser.cpp ${srcroot}/model/object_list.cpp ${srcroot}/config_file.cpp ${srcroot}/config_file_parser.cpp ${srcroot}/scim.cpp ${srcroot}/utility/utils.cpp ${srcroot}/utility/utils.hpp ${srcroot}/utility/utils.cpp ${srcroot}/ldap_wrapper.cpp ${srcroot}/ldap_wrapper.hpp ${srcroot}/data_server.cpp ${srcroot}/data_server.hpp ${srcroot}/local_id_store.cpp ${srcroot}/local_id_store.hpp ${srcroot}/scim_data.cpp ${srcroot}/scim_data.hpp ${srcroot}/json_data_file.cpp ${srcroot}/json_data_file.hpp)

add_executable(EgilSCIMClient ${SOURCES} ${srcroot}/simplescim_ldap.cpp ${srcroot}/main.cpp)
#add_executable(EgilSCIMClientPrint ${SOURCES} ${srcroot}/simplescim_ldap.cpp ${srcroot}/utility/simplescim_cache_file_print.cpp)

install (TARGETS EgilSCIMClient DESTINATION bin)
#install (TARGETS EgilSCIMClientPrint DESTINATION bin)

