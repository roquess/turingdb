
set(core_sources 
            LabelIDMap.cpp
            LabelIndex.cpp
            Storage.cpp
            StorageAccessor.cpp
            NodeAccessor.cpp
            EdgeAccessor.cpp
            Node.cpp
            Edge.cpp
            EdgeMap.cpp
            DB.cpp)

add_library(turing_db_core_s STATIC ${core_sources})
target_include_directories(turing_db_core_s PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_include_directories(turing_db_core_s PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/columns)
target_link_libraries(turing_db_core_s
    common_s)
