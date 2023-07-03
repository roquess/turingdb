set(library_sources
            TypeLibrary.cpp
            TuringStdTypeLibrary.cpp)

add_library(turing_db_library_s
    STATIC  ${library_sources})

target_include_directories(turing_db_library_s
    PUBLIC  ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(turing_db_library_s
    PUBLIC  turing_db_core_s
    PRIVATE common_s
            turing_log_s)
