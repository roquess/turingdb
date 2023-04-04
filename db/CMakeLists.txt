
set(db_sources 
            DB.cpp
            Network.cpp
            Component.cpp
            Node.cpp
            Edge.cpp

            ComponentStorage.cpp
            ComponentColumn.cpp

            Value.cpp

            ValueType.cpp

            StringIndex.cpp
            SharedString.cpp
            StringRef.cpp)

add_library(turing_db_s STATIC ${db_sources})
target_include_directories(turing_db_s PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_include_directories(turing_db_s PRIVATE ${BIO_SOURCE_DIR}/messages)
