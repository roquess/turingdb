set(SAMPLE_NAME scan_nodes_processor)
set(SOURCES     main.cpp)

turing_sample(${SAMPLE_NAME} ${SOURCES})

target_link_libraries(${SAMPLE_NAME}
    PRIVATE turing_db2_core_s
            turing_db2_pipeline_s)
