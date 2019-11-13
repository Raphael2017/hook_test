cmake_minimum_required(VERSION 3.13)
project(hook)

set(CMAKE_CXX_STANDARD 14)

set(CMAKE_CXX_FLAGS "-g ${CMAKE_CXX_FLAGS}")

add_subdirectory(math)
add_subdirectory(math_n)

include_directories(math)

add_executable(hook main.cpp)

target_link_libraries(hook math_add)
target_link_libraries(hook dl)
