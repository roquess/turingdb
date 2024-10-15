
set(http_sources
        Utils.cpp
        Server.cpp)

add_library(turing_http_s STATIC ${http_sources})

target_include_directories(turing_http_s PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(turing_http_s PRIVATE spdlog::spdlog)
