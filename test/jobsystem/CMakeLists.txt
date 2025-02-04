add_turing_test(jobsystem_test JobSystemTest.cpp)

target_link_libraries(jobsystem_test
    PRIVATE turing_jobsystem_s)

target_include_directories(jobsystem_test
    PRIVATE ${CMAKE_CURRENT_LIST_DIR})

