
set(report_sources GraphReport.cpp)

add_library(turing_db_report_s STATIC ${report_sources})
target_include_directories(turing_db_report_s PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(turing_db_report_s
    PRIVATE turing_db_storage_s)
