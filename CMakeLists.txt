cmake_minimum_required(VERSION 3.20)
project(NEAT)

set(CMAKE_CXX_STANDARD 11)

add_executable(NEAT main.cpp NEAT/individual.cpp NEAT/individual.h NEAT/hyperSingleton.cpp NEAT/hyperSingleton.h)

add_subdirectory(Google_tests)