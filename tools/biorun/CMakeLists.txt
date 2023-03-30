
set(biorun_sources 
            NotebookRunner.cpp
            BioRun.cpp)

add_executable(biorun ${biorun_sources})
target_link_libraries(biorun PRIVATE
    tools_common_s)

target_include_directories(biorun PRIVATE ${BIO_SOURCE_DIR}/messages)

install(TARGETS biorun DESTINATION bin)
