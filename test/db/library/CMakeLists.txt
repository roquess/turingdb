function(add_db_library_gtest exe_name cpp_file)
    add_executable(${exe_name} ${cpp_file})

    target_link_libraries(${exe_name} PRIVATE
        turing_db_library_s
        gtest_main)

    gtest_discover_tests(${exe_name})
endfunction()

add_db_library_gtest(turing_std_type_library_tests TuringStdTypeLibraryTest.cpp)
