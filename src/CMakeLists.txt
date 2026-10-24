# CMakeList.txt : CMake project for CMakeProject, include source and define project specific logic here.
#

cmake_minimum_required (VERSION 3.8)

set(LOGENGINE_LIB "LogEngine2Lib")
project(${LOGENGINE_LIB} LANGUAGES CXX)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Debug" CACHE STRING "Default build type: Debug" FORCE)
endif()

file(GLOB INCLUDE_FILES CONFIGURE_DEPENDS "*.h")
file(GLOB SRC_FILES CONFIGURE_DEPENDS "*.cpp")

set(target ${LOGENGINE_LIB})

add_library(${target} STATIC ${SRC_FILES} ${INCLUDE_FILES} )

target_compile_features(${LOGENGINE_LIB} PUBLIC cxx_std_20)

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    # Add "_DEBUG" definition to the source code.
    target_compile_definitions(${LOGENGINE_LIB} PRIVATE _DEBUG)
else()
    # Add "NDEBUG" definition to the source code.
    target_compile_definitions(${LOGENGINE_LIB} PRIVATE NDEBUG)
endif()



