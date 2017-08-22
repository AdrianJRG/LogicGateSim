project(LogicGateSim C)
cmake_minimum_required(VERSION 3.7)
set(CMAKE_C_STANDARD 99)

set(TARGET_GROUP production CACHE STRING "Group to build")

# Modules that exist in both our release and our unit tests, are added now
add_subdirectory(src)

if(TARGET_GROUP STREQUAL production)

elseif(TARGET_GROUP STREQUAL test)
  include(CTest)

  add_subdirectory(external) 
  add_subdirectory(test) 
else() 
  message(FATAL_ERROR "Given TARGET_GROUP unknown")
endif()

# Let's tell it a bit about our environment
#SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)
