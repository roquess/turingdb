set(SAMPLE_NAME index-serialisation)
set(SOURCES
    main.cpp
)

turing_sample(${SAMPLE_NAME} ${SOURCES})

target_link_libraries(${SAMPLE_NAME} PRIVATE
    turing_common_s
    turing_db_interpreter_s
    turing_db_s
    turing_db_memory_s
    turing_db_storage_s
    turing_db_dump_s
    turing_db_examples_s
)
