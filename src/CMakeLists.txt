#set(SOURCE_FILES main.c core.c core.h IOfile.c IOfile.h simulator.c simulator.h)
#add_executable(LogicGateSim ${SOURCE_FILES})

add_library(IOfile STATIC
    IOfile.h
	IOfile.c
)

target_include_directories(IOfile PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}
)

add_library(core STATIC
    core.h
    core.c
)

target_include_directories(core PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}
)

add_library(simulator STATIC
    simulator.h
	simulator.c
)

target_include_directories(simulator PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}
)

#file(GLOB SOURCES ./*.c ./*.h)
#add_executable(release ${SOURCES})
#
#target_include_directories(release PUBLIC 
#    ${CMAKE_CURRENT_LIST_DIR}
#)
