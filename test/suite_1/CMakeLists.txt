add_executable(suite_1_app
    TestIOFile.c
)

target_link_libraries(suite_1_app
    IOfile
    core
    simulator	
    Unity
)

add_test(suite_1_test suite_1_app)
