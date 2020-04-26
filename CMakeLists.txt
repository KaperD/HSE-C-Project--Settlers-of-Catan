project(Catan)
cmake_minimum_required(VERSION 3.2)

add_compile_options(-std=c++17)

# GRPC and Protocol Buffers libraries location
list(APPEND CMAKE_PREFIX_PATH "/usr/local")

# Cmake find modules
#list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/Network/cmake")
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_CURRENT_LIST_DIR}/Network/cmake")

# SDL2
# Могут понадобиться следующие команды
# sudo apt-get install libsdl-image1.2-dev
# sudo apt-get install libsdl-dev
find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS} ${SDL2_IMAGE_INCLUDE_DIRS})

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

set(
    grpc
    Network/game.grpc.pb.cc
    Network/game.grpc.pb.h
    Network/game.pb.cc
    Network/game.pb.h
)


# Building project
add_executable(project
    ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
    ${grpc}
    ${model}
    ${controller}
    ${client}
    ${spinlock}
    ${random}
    ${gui}
)

target_link_libraries(project
    ${EXECUTABLE_NAME}
    ${SDL2_IMAGE_LIBRARIES}
    ${SDL2_LIBRARIES}
    ${CMAKE_SOURCE_DIR}/Libs/libgrpc++.so.1
    ${CMAKE_SOURCE_DIR}/Libs/libprotobuf.so.22
)

include_directories(Client)
include_directories(Model)
include_directories(Controller)
include_directories(Network)
include_directories(GUI)
include_directories(GUI/Image)
include_directories(Utility)

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR})

target_compile_options(project PRIVATE)
