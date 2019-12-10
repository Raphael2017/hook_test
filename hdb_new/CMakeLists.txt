include_directories(../hdb)
include_directories(./include)
include_directories(../subhook)
include_directories(../funchook)

aux_source_directory(./src SRCS)
add_library(hdb_new SHARED ./src/hdb_new.cpp ./src/talk_with_hdb.cpp
        ./src/TableMetaDataImpl.cpp ./src/tool.cpp ./src/HookImpl.cpp ./src/EnforcerCtxImpl.cpp
        ./src/MaskStrategyImpl.cpp
        ./src/dump.cpp)
target_link_libraries(hdb_new dl)
target_link_libraries(hdb_new /home/qwerty/github/hook_test/subhook/libsubhook.so)
target_link_libraries(hdb_new /home/qwerty/github/hook_test/funchook/libfunchook.so)