add_executable(gml_dump_tests GMLDumperTest.cpp)
target_link_libraries(gml_dump_tests PRIVATE
    bio_gmldump_s
    turing_db_core_s
    gtest_main)

gtest_discover_tests(gml_dump_tests)
