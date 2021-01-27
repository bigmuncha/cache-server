cmake_minimum_required(VERSION 3.10)

project(cache-server)

set(SOURCE_EXE main.cpp)

#set(HASH_TABLE_LIB hash_table.cpp)
#set(REQUEST_PARSE_LIB req_parse.cpp)

set (sock_includes "./socket_transfer/set_nonblock.h" "./socket_transfer/fd_pass.h" )
set (sock_src "./socket_transfer/set_nonblock.c" "./socket_transfer/fd_pass.c")

set(hash_table_includes "./hash_table/hash_table.hpp")
set(hash_table_src "./hash_table/hash_table.cpp")

set(req_parse_includes "./req_parse/req_parse.hpp")
set(req_parse_src "./req_parse/req_parse.cpp")

#add_library(hash_table ${HASH_TABLE_LIB})
#add_library(request_parse ${REQUEST_PARSE_LIB})

find_package(Boost REQUIRED COMPONENTS  filesystem thread system regex)
if(Boost_FOUND)
  include_directories(${Boost_INCLUDE_DIRS} "./socket_transfer" "./hash_table" "./req_parse")

add_executable (cache_serv ${SOURCE_EXE} ${sock_includes} ${sock_src} ${hash_table_includes} ${hash_table_src} ${req_parse_includes} ${req_parse_src})


target_link_libraries(cache_serv  ${Boost_LIBRARIES})
endif()
