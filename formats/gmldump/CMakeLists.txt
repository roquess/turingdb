
set(gmldump_sources GMLDumper.cpp)

add_library(bio_gmldump_s STATIC ${gmldump_sources})
target_include_directories(bio_gmldump_s PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(bio_gmldump_s PRIVATE
    turing_db_s)
