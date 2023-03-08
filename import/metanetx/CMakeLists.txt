
set(import_metanetx_sources MetaNetXLoader.cpp)

add_library(bio_import_metanetx_s STATIC ${import_metanetx_sources})

target_link_libraries(bio_import_metanetx_s
    bio_common_s
    bio_formats_s
    bio_db_core_s)

target_include_directories(bio_import_metanetx_s PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
