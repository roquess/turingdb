
set(db_sources 
            SharedString.cpp
            StringRef.cpp
            StringIndex.cpp
            LinkType.cpp
            Object.cpp
            PropertyType.cpp
            ValueType.cpp
            Value.cpp
            Link.cpp
            Network.cpp
            ObjectType.cpp
            PropertyStore.cpp
            LinkStore.cpp
            DB.cpp)

add_library(turing_db_s STATIC ${db_sources})
target_include_directories(turing_db_s PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_include_directories(turing_db_s PRIVATE ${BIO_SOURCE_DIR}/messages)
