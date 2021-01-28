cmake_minimum_required(VERSION 3.10)

project(cache-server)

set(SOURCE_EXE main.cpp)

#set(HASH_TABLE_LIB hash_table.cpp)
#set(REQUEST_PARSE_LIB req_parse.cpp)

set(CMAKE_CXX_STANDARD 14)

set (fd_pass_includes  "./socket_transfer/fd_pass.h" )
set (fd_pass_src  "./socket_transfer/fd_pass.cc")

set (nonblock_includes "./socket_transfer/set_nonblock.h")
set (nonblock_src "./socket_transfer/set_nonblock.cc")

set(hash_table_includes "./hash_table/hash_table.hpp")
set(hash_table_src "./hash_table/hash_table.cpp")

set(req_parse_includes "./req_parse/req_parse.hpp")
set(req_parse_src "./req_parse/req_parse.cpp")

#add_library(hash_table ${HASH_TABLE_LIB})
#add_library(request_parse ${REQUEST_PARSE_LIB})

find_package(Boost REQUIRED COMPONENTS  filesystem thread system regex)
if(Boost_FOUND)
  include_directories(${Boost_INCLUDE_DIRS} "./socket_transfer" "./hash_table" "./req_parse")

add_executable (cache_serv ${SOURCE_EXE} ${fd_pass_includes}
  ${fd_pass_src} ${hash_table_includes} ${hash_table_src}
  ${req_parse_includes} ${req_parse_src} ${nonblock_includes} ${nonblock_src})


target_link_libraries(cache_serv  ${Boost_LIBRARIES} )
endif()
