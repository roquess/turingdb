set(proto_schema ${TURING_DB_PROTO_DIR}/DBService.proto)
set(proto_srcs ${CMAKE_CURRENT_BINARY_DIR}/DBService.pb.cc)
set(proto_headers ${CMAKE_CURRENT_BINARY_DIR}/DBService.pb.h)
set(proto_grpc_srcs ${CMAKE_CURRENT_BINARY_DIR}/DBService.grpc.pb.cc)
set(proto_grpc_headers ${CMAKE_CURRENT_BINARY_DIR}/DBService.grpc.pb.h)

# Generate grpc cpp files
add_custom_command(
    OUTPUT ${proto_srcs} ${proto_headers} ${proto_grpc_srcs} ${proto_grpc_headers}
    COMMAND ${Protobuf_PROTOC_EXECUTABLE}
    ARGS --grpc_out ${CMAKE_CURRENT_BINARY_DIR}
        --cpp_out ${CMAKE_CURRENT_BINARY_DIR}
        -I ${TURING_DB_PROTO_DIR}
        --plugin=protoc-gen-grpc=$<TARGET_FILE:gRPC::grpc_cpp_plugin>
        ${proto_schema}
    DEPENDS ${proto_schema})

add_library(turing_db_proto_s STATIC ${proto_srcs} ${proto_grpc_srcs})
target_include_directories(turing_db_proto_s PUBLIC ${CMAKE_CURRENT_BINARY_DIR})
target_link_libraries(turing_db_proto_s PUBLIC gRPC::grpc++)

# PIC version
add_library(turing_db_proto_fpic_s STATIC ${proto_srcs} ${proto_grpc_srcs})
target_include_directories(turing_db_proto_fpic_s PUBLIC ${CMAKE_CURRENT_BINARY_DIR})
target_link_libraries(turing_db_proto_fpic_s PUBLIC gRPC::grpc++)
set_target_properties(turing_db_proto_fpic_s PROPERTIES
    POSITION_INDEPENDENT_CODE ON)
