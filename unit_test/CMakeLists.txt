include_directories(../microtest)
include_directories(../hdb_new/include)
include_directories(../hdb)

add_executable(unit_test ./test.cpp)

target_link_libraries(unit_test /home/qwerty/github/hook_test/hdb/libSQLDBCHDB.so)
target_link_libraries(unit_test hdb_new)