add_library(simulator STATIC
        simulator.c
        simulator.h)


target_include_directories(simulator PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}
)
