add_library(iofile STATIC
        iofile.c
        iofile.h)


target_include_directories(iofile PUBLIC
        ${CMAKE_CURRENT_LIST_DIR}
        )
