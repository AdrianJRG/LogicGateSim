add_executable(logicsimlib_module_app
        logicsimlib-test.c
        logicsimlib-test.h
        )

target_link_libraries(logicsimlib_module_app
        logicgatelib
        Unity
        )

add_test(logicsimlib_module_test logicsimlib_module_app)