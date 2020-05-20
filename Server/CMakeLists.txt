project(Server)
cmake_minimum_required(VERSION 3.2)

add_compile_options(-std=c++17)

# GRPC and Protocol Buffers libraries location
list(APPEND CMAKE_PREFIX_PATH "/usr/lib" "/usr/local/include/google/protobuf")

# Cmake find modules
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake")

find_package(Protobuf REQUIRED)
find_package(GRPC REQUIRED)

set(PROTOS
    ${CMAKE_CURRENT_SOURCE_DIR}/game.proto
)

set(PROTO_SRC_DIR ${CMAKE_CURRENT_BINARY_DIR}/Protos)
file(MAKE_DIRECTORY ${PROTO_SRC_DIR})
include_directories(${PROTO_SRC_DIR})

protobuf_generate_cpp(PROTO_SRCS PROTO_HDRS ${PROTO_SRC_DIR} ${PROTOS})
grpc_generate_cpp(GRPC_SRCS GRPC_HDRS ${PROTO_SRC_DIR} ${PROTOS})

set(
    spinlock
    ../Utility/spinlock.cpp
    ../Utility/spinlock.h
)

add_executable(server
    ${CMAKE_CURRENT_SOURCE_DIR}/Server.cpp
    ${PROTO_SRCS}
    ${GRPC_SRCS}
    ${spinlock}
)
target_link_libraries(server
    gRPC::grpc++_reflection
    protobuf::libprotobuf
#    /usr/local/lib/libgrpc++_unsecure.a
#    /usr/local/lib/libgrpc.a
#    /usr/local/lib/libgrpc++.a
#    /usr/local/lib/libprotobuf.a
#    /lib/x86_64-linux-gnu/libpthread.so.0
)

include_directories(../Utility)
include_directories(Protos)