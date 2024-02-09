
add_executable(db2_pipeline PipelineTest.cpp)
target_link_libraries(db2_pipeline PRIVATE
    turing_db2_core_s
    turing_db2_pipeline_s
    gtest_main)
gtest_discover_tests(db2_pipeline)

