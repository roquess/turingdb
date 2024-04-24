
set(tools_common_s
        ToolInit.cpp)

add_library(tools_common_s STATIC ${tools_common_s})

target_link_libraries(tools_common_s
    common_s
    spdlog
    argparse)

target_include_directories(tools_common_s PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
