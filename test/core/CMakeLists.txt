add_executable(core_app
    core-test.c
)

target_link_libraries(core_app
    IOfile
    core
    simulator
    Unity
)

add_test(core_test core_app)
