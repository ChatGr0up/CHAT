cmake_minimum_required(VERSION 3.16)
project(proto_module)

find_package(Protobuf REQUIRED)

file(GLOB PROTO_FILES "${CMAKE_CURRENT_SOURCE_DIR}/*.proto")

if(NOT PROTO_FILES)
    message(FATAL_ERROR "No .proto files found in ${CMAKE_CURRENT_SOURCE_DIR}")
endif()

protobuf_generate_cpp(PROTO_SRCS PROTO_HDRS ${PROTO_FILES})

add_library(proto_module STATIC ${PROTO_SRCS} ${PROTO_HDRS})

target_include_directories(proto_module PUBLIC ${CMAKE_CURRENT_BINARY_DIR})

target_link_libraries(proto_module PUBLIC protobuf::libprotobuf)

set_target_properties(proto_module PROPERTIES
    LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib
    ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib
)