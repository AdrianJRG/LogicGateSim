cmake_minimum_required(VERSION 3.7)
project(LogicGateSim)

set(CMAKE_C_STANDARD 99)

set(SOURCE_FILES main.c simulator.c simulator.h)
add_executable(LogicGateSim ${SOURCE_FILES})