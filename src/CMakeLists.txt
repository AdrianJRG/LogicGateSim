#set(SOURCE_FILES main.c core.c core.h iofile.c iofile.h simulator.c simulator.h)
#add_executable(LogicGateSim ${SOURCE_FILES})

add_subdirectory(logicgatelib)
add_subdirectory(simulator)
add_subdirectory(iofile)
add_subdirectory(core)

file(GLOB SOURCES ${SOURCES} ./*.c ./*.h)
add_executable(release ${SOURCES})

target_include_directories(release PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}
)

target_link_libraries(release
        core
        iofile
        logicgatelib
        simulator
)
