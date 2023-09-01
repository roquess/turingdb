
set(dbms_sources DBManager.cpp)

add_library(turing_db_manager_s STATIC ${dbms_sources})

target_include_directories(turing_db_manager_s PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(turing_db_manager_s PRIVATE
    turing_db_core_s
    turing_db_dumper_s
    common_s)
