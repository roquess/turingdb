
set(tools_common_s
        ToolInit.cpp
        ArgParser.cpp)

add_library(tools_common_s STATIC ${tools_common_s})

target_link_libraries(tools_common_s
    turing_log_s
    common_s)

target_include_directories(tools_common_s PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_include_directories(tools_common_s PRIVATE ${BIO_SOURCE_DIR}/messages)
