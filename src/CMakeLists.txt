#set(SOURCE_FILES main.c core.c core.h IOfile.c IOfile.h simulator.c simulator.h)
#add_executable(LogicGateSim ${SOURCE_FILES})

file(GLOB SOURCES ./*.c ./*.h)
add_executable(release ${SOURCES})
target_include_directories(release PUBLIC ${CMAKE_CURRENT_LIST_DIR})
