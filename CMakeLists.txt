cmake_minimum_required(VERSION 3.10)

project(cache-server)

set(SOURCE_EXE main.cpp)

set(HASH_TABLE_LIB hash_table.cpp)

add_library(hash_table ${HASH_TABLE_LIB})

find_package(Boost REQUIRED COMPONENTS  filesystem thread system)
if(Boost_FOUND)
  include_directories(${Boost_INCLUDE_DIRS})

add_executable (cache_serv ${SOURCE_EXE})

target_link_libraries(cache_serv hash_table ${Boost_LIBRARIES})
endif()
