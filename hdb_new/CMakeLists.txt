include_directories(../hdb)
include_directories(../subhook)

set(CMAKE_CXX_FLAGS "-D_AMD64_ -D_UNIX64 ${CMAKE_CXX_FLAGS}")
add_library(hdb_new SHARED ./hdb_new.cpp)
target_link_libraries(hdb_new dl)
target_link_libraries(hdb_new /home/qwerty/github/hook_test/subhook/libsubhook.so)