project(Catan)
cmake_minimum_required(VERSION 3.2)

set (CMAKE_CXX_STANDARD 17)

# GRPC and Protocol Buffers libraries location
list(APPEND CMAKE_PREFIX_PATH "/usr/local")

# Cmake find modules
#list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/Network/cmake")
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_CURRENT_LIST_DIR}/Network/cmake")

find_package(Protobuf REQUIRED)
find_package(GRPC REQUIRED)

set(PROTOS
    ${CMAKE_CURRENT_SOURCE_DIR}/Network/game.proto
)

set(PROTO_SRC_DIR ${CMAKE_CURRENT_BINARY_DIR}/Network)
file(MAKE_DIRECTORY ${PROTO_SRC_DIR})
include_directories(${PROTO_SRC_DIR})

protobuf_generate_cpp(PROTO_SRCS PROTO_HDRS ${PROTO_SRC_DIR} ${PROTOS})
grpc_generate_cpp(GRPC_SRCS GRPC_HDRS ${PROTO_SRC_DIR} ${PROTOS})

# SDL2
# Могут понадобиться следующие команды
# sudo apt-get install libsdl-image1.2-dev
# sudo apt-get install libsdl-dev
find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)
find_package(SDL2_mixer REQUIRED)
find_package(SDL2_ttf REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS} ${SDL2_IMAGE_INCLUDE_DIRS} ${SDL2_MIXER_INCLUDE_DIRS} ${SDL2_TTF_INCLUDE_DIRS})

set(
    client
    Client/Client.cpp
    Client/Client.h
)

set(
    controller
    Controller/GameController.cpp
    Controller/GameController.h
)

set(
    model
    Model/Board.cpp
    Model/Board.h
)

set(
    spinlock
    Utility/spinlock.cpp
    Utility/spinlock.h
)

set(
    random
    Utility/random.cpp
    Utility/random.h
)

set(
    gui
    GUI/sdl.cpp
    GUI/sdl.h
)

# Building project
add_executable(project
    ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
    ${PROTO_SRCS}
    ${GRPC_SRCS}
    ${model}
    ${controller}
    ${client}
    ${spinlock}
    ${random}
    ${gui}
        Network/LocalServer.h)

target_link_libraries(project
    ${EXECUTABLE_NAME}
    ${SDL2_IMAGE_LIBRARIES}
    ${SDL2_LIBRARIES}
    ${SDL2_MIXER_LIBRARIES}
    ${SDL2_TTF_LIBRARIES}
    # /usr/local/lib/libgrpc++_unsecure.a
    # /usr/local/lib/libgrpc.a
    # /usr/local/lib/libgrpc++.a
    # /usr/local/lib/libprotobuf.a
    # /lib/x86_64-linux-gnu/libpthread.so.0
    gRPC::grpc++_reflection
    protobuf::libprotobuf
)

include_directories(Client)
include_directories(Model)
include_directories(Controller)
include_directories(Network)
include_directories(GUI)
include_directories(Utility)

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR})

target_compile_options(project PRIVATE)

