
set(bio_formats_sources RowReader.cpp)

add_library(bio_formats_s STATIC ${bio_formats_sources})
target_include_directories(bio_formats_s PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
