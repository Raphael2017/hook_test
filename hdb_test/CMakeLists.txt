include_directories(../hdb)
#include_directories(../../subhook)
add_executable(hdb_test ./main.cpp)
target_link_libraries(hdb_test /home/qwerty/github/hook_test/hdb/libSQLDBCHDB.so)
target_link_libraries(hdb_test dl)
#target_link_libraries(hdb_test SQLDBCHDB)

#target_link_libraries(hdb_test /home/qwerty/github/subhook/libsubhook.so)
