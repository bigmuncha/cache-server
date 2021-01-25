cmake_minimum_required(VERSION 3.10)

project(cache-server)

set(SOURCE_EXE main.cpp)

set(HASH_TABLE_LIB hash_table.cpp)
set(REQUEST_PARSE_LIB req_parse.cpp)

add_library(hash_table ${HASH_TABLE_LIB})
add_library(request_parse ${REQUEST_PARSE_LIB})

find_package(Boost REQUIRED COMPONENTS  filesystem thread system regex)
if(Boost_FOUND)
  include_directories(${Boost_INCLUDE_DIRS})

add_executable (cache_serv ${SOURCE_EXE})

target_link_libraries(cache_serv hash_table request_parse ${Boost_LIBRARIES})
endif()
