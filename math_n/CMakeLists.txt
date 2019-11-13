include_directories(../math)
add_library(math_add_n SHARED ./add.cpp ./gothook.cpp)
target_link_libraries(math_add_n dl)