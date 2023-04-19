set(bioimport_sources BioImport.cpp)

add_executable(bioimport ${bioimport_sources})

target_link_libraries(bioimport
    turing_gml_s
    tools_common_s)

target_include_directories(bioimport PRIVATE ${BIO_SOURCE_DIR}/messages)

install(TARGETS bioimport DESTINATION bin)
