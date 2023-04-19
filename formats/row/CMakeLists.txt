
set(row_sources RowReader.cpp)

add_library(bio_row_s STATIC ${row_sources})
target_include_directories(bio_row_s PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
