
set(wrt_sources
        RegressJob.cpp
        RegressTesting.cpp
        wrt.cpp)

add_executable(wrt ${wrt_sources})

target_link_libraries(wrt PRIVATE
    common_s
    tools_common_s)

target_include_directories(wrt PRIVATE ${BIO_SOURCE_DIR}/messages)

install(TARGETS wrt DESTINATION bin)
