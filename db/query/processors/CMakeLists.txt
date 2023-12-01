
set(processors_sources
        Port.cpp
        Processor.cpp)

add_library(turing_db_query_processors_s STATIC ${processors_sources})
