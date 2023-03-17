
set(biocauldron_sources BioCauldron.cpp)

add_executable(biocauldron ${biocauldron_sources})
target_link_libraries(biocauldron PRIVATE
    turing_ui_server_s
    tools_common_s)

install(TARGETS biocauldron DESTINATION bin)
