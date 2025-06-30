# CMakeList.txt : CMake project for CMakeProject, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

#set(PROJECT_NAME "LogEngine2Example")

# ${EXAMPLE_APP} var is defined in upper CMakeLists.txt file
project(${EXAMPLE_APP} LANGUAGES CXX)

file(GLOB examplesrc "*.cpp")

set(target ${EXAMPLE_APP})
add_executable(${target} ${examplesrc})

target_link_libraries(${target} ${LOGENGINE_LIB})
target_link_libraries(${target} version.lib)



