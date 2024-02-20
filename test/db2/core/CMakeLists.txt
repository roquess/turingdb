
add_executable(db2_label_map LabelMapTest.cpp)
target_link_libraries(db2_label_map PRIVATE
    turing_db2_core_s
    gtest_main)
gtest_discover_tests(db2_label_map)

