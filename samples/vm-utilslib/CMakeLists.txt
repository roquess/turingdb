set(vm_utilslib_sources
            vmutils.cpp)

add_library(turing_samples_vm_utilslib_s
    STATIC  ${vm_utilslib_sources})

target_include_directories(turing_samples_vm_utilslib_s
    PUBLIC  ${CMAKE_CURRENT_SOURCE_DIR}
    PRIVATE ${CMAKE_CURRENT_BINARY_DIR})

target_link_libraries(turing_samples_vm_utilslib_s
    PUBLIC  common_s
            turing_db2_system_s
            turing_db2_core_s
            turing_db2_import_neo4j_s
            turing_db2_vm_s)
