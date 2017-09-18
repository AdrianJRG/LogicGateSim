add_library(logicgatelib STATIC
        logicgatelib.h
        logicgatelib.c)

target_include_directories(logicgatelib PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}
)
