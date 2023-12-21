# Copy test data to binary directory
configure_file(example.csv ${CMAKE_CURRENT_BINARY_DIR}/example.csv COPYONLY)

function(add_import_csv_gtest exe_name cpp_file)
    add_executable(${exe_name} ${cpp_file})
    target_link_libraries(${exe_name} PRIVATE
        turing_import_csv_s
        turing_db_core_s
        turing_log_s
        gtest_main
    )
    gtest_discover_tests(${exe_name})
endfunction()

add_import_csv_gtest(import_csv_tests CSVImportTest.cpp)
