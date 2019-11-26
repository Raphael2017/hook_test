cmake_minimum_required(VERSION 3.13)
project(hook)

set(CMAKE_CXX_STANDARD 14)

set(CMAKE_CXX_FLAGS "-g -no-pie -O0 ${CMAKE_CXX_FLAGS}")

add_subdirectory(math)
add_subdirectory(math_n)
add_subdirectory(hdb_test)
add_subdirectory(hdb_new)

include_directories(math)


add_executable(hook main.cpp)

target_link_libraries(hook math_add)
target_link_libraries(hook dl)

#target machine 10.23.56.93 admin 123blue!
