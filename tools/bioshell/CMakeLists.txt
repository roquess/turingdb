
set(bioshell_sources BioShell.cpp)

add_executable(bioshell ${bioshell_sources})

target_link_libraries(bioshell PRIVATE
    turing_client_native_s
    common_s
    tools_common_s
    linenoise_s)

target_include_directories(bioshell PRIVATE ${BIO_SOURCE_DIR}/messages)

install(TARGETS bioshell DESTINATION bin)
