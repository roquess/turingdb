set(http_parser_sources HTTPParser.cpp)

add_library(turing_http_parser_s STATIC ${http_parser_sources})

target_include_directories(turing_http_parser_s
  PUBLIC  ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(turing_http_parser_s
  PRIVATE spdlog::spdlog
          turing_log_s
          common_s)
