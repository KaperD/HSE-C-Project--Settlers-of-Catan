project(Server)
cmake_minimum_required(VERSION 3.2)

add_compile_options(-std=c++17)

set(
    spinlock
    ../Utility/spinlock.cpp
    ../Utility/spinlock.h
)

set(
    grpc
    ../Network/game.grpc.pb.cc
    ../Network/game.grpc.pb.h
    ../Network/game.pb.cc
    ../Network/game.pb.h
)

add_executable(server
    ${CMAKE_CURRENT_SOURCE_DIR}/Server.cpp
    ${grpc}
    ${spinlock}
)
target_link_libraries(server
    ${CMAKE_SOURCE_DIR}/../Libs/libgrpc++.so.1
    ${CMAKE_SOURCE_DIR}/../Libs/libprotobuf.so.22  
)

include_directories(../Utility)
include_directories(../Network)
