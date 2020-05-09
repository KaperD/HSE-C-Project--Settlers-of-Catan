project(ModelTests)
cmake_minimum_required(VERSION 3.2)

add_compile_options(-std=c++17)

find_package(Threads REQUIRED)
find_package(GTest REQUIRED)

add_executable(
        ModelTest
        Test/run_tests.cpp
        Test/testGraph.cpp
        Test/testCanBuild.cpp
        Test/testHexes.cpp
        Test/testRoadsRecord.cpp
        ../Utility/random.cpp
        ../Utility/random.h
        ${CMAKE_CURRENT_SOURCE_DIR}/Board.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/Board.h
)

target_include_directories(
        ModelTest
        PRIVATE
        ${GTEST_INCLUDE_DIRS}
)

target_link_libraries(
        ModelTest
        GTest::GTest
        Threads::Threads
)

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR})

target_compile_options(ModelTest PRIVATE -Werror -Wall -Wextra)

include_directories(../Utility)
