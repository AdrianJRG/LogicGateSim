add_executable(IOfile_app
        IOfile-test.c
)

target_link_libraries(IOfile_app
    IOfile
    Unity
)

add_test(IOfile_test IOfile_app)
