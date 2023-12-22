# Copy test data to binary directory
configure_file(1_empty_line_end.csv ${CMAKE_CURRENT_BINARY_DIR}/1_empty_line_end.csv COPYONLY)
configure_file(2_empty_line_end.csv ${CMAKE_CURRENT_BINARY_DIR}/2_empty_line_end.csv COPYONLY)
configure_file(3_empty_line_end.csv ${CMAKE_CURRENT_BINARY_DIR}/3_empty_line_end.csv COPYONLY)
configure_file(one_column.csv ${CMAKE_CURRENT_BINARY_DIR}/one_column.csv COPYONLY)
configure_file(whitespaces.csv ${CMAKE_CURRENT_BINARY_DIR}/whitespaces.csv COPYONLY)
configure_file(only_strings.csv ${CMAKE_CURRENT_BINARY_DIR}/only_strings.csv COPYONLY)
configure_file(example1.csv ${CMAKE_CURRENT_BINARY_DIR}/example1.csv COPYONLY)
configure_file(example2.csv ${CMAKE_CURRENT_BINARY_DIR}/example2.csv COPYONLY)
configure_file(strings_and_floats.csv ${CMAKE_CURRENT_BINARY_DIR}/strings_and_floats.csv COPYONLY)
configure_file(duplicates.csv ${CMAKE_CURRENT_BINARY_DIR}/duplicates.csv COPYONLY)
configure_file(duplicates_in_header.csv ${CMAKE_CURRENT_BINARY_DIR}/duplicates_in_header.csv COPYONLY)
configure_file(node_redefinition.csv ${CMAKE_CURRENT_BINARY_DIR}/node_redefinition.csv COPYONLY)
configure_file(example_semicolon.csv ${CMAKE_CURRENT_BINARY_DIR}/example_semicolon.csv COPYONLY)
configure_file(missing_entry.csv ${CMAKE_CURRENT_BINARY_DIR}/missing_entry.csv COPYONLY)
configure_file(too_many_entries.csv ${CMAKE_CURRENT_BINARY_DIR}/too_many_entries.csv COPYONLY)
configure_file(missing_cell_value.csv ${CMAKE_CURRENT_BINARY_DIR}/missing_cell_value.csv COPYONLY)


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
