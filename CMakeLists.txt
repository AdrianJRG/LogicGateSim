cmake_minimum_required(VERSION 3.7)
project(LogicGateSim)

set(CMAKE_C_STANDARD 99)

set(SOURCE_FILES main.c core.c core.h IOfile.c IOfile.h simulator.c simulator.h)
add_executable(LogicGateSim ${SOURCE_FILES})