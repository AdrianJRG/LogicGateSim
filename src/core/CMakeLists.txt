add_library(core STATIC
        core.c
        core.h)


target_include_directories(core PUBLIC
        ${CMAKE_CURRENT_LIST_DIR}
        )
