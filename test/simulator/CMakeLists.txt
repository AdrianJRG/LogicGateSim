add_executable(simulator_app
    simulator-test.c
)

target_link_libraries(simulator_app
    simulator
    Unity
)

add_test(simulator_test simulator_app)
