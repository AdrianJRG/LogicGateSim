add_executable(IOfile_app
        IOfile-test.c
)

target_link_libraries(IOfile_app
    IOfile
    Unity
)

add_test(IOfile_test IOfile_app)

#for new one
add_executable(iofile_module_app
        iofile-lib-test.c
        iofile-lib-test.h
        )

target_link_libraries(iofile_module_app
        iofile
        Unity
)

add_test(iofile_module_test iofile_module_app)