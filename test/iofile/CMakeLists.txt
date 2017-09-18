add_executable(iofile_module_app
        iofile-lib-test.c
        iofile-lib-test.h
        )

target_link_libraries(iofile_module_app
        iofile
        Unity
        logicgatelib
)

add_test(iofile_module_test iofile_module_app)