project(Catan)
cmake_minimum_required(VERSION 3.2)

add_compile_options(-std=c++17)

# Cmake find modules
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/Network/cmake")

find_package(GRPC REQUIRED)

file(MAKE_DIRECTORY ${PROTO_SRC_DIR})
include_directories(${PROTO_SRC_DIR})

# SDL2
# Могут понадобиться следующие команды
# sudo apt-get install libsdl-image1.2-dev
# sudo apt-get install libsdl-dev
find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS})

add_library(
    client
    Client/Client.cpp
    Client/Client.h
)

add_library(
    controller
    Controller/GameController.cpp
    Controller/GameController.h
)

add_library(
    model
    Model/Board.cpp
    Model/Board.h
)


# Building project
add_executable(project
    ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
    GUI/sdl.cpp
    ${PROTO_SRCS}
    ${GRPC_SRCS}
)

target_link_libraries(project
    ${SDL2_LIBRARIES}
    gRPC::grpc++
    model
    controller
    client
)

include_directories(Client)
include_directories(Model)
include_directories(Controller)
include_directories(Network)
include_directories(GUI)

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR})

target_compile_options(project PRIVATE -Werror -Wall -Wextra)
