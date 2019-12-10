cmake_minimum_required(VERSION 3.3)
project(hook)

set(CMAKE_CXX_STANDARD 14)

set(CMAKE_CXX_FLAGS "-g ${CMAKE_CXX_FLAGS}")

add_subdirectory(hdb_test)
add_subdirectory(hdb_new)
add_subdirectory(unit_test)


#target machine 10.23.56.93 admin 123blue!
