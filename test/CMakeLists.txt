cmake_minimum_required (VERSION 3.8)

#set(CMAKE_C_COMPILER gcc-13)
#set(CMAKE_CXX_COMPILER g++-13)

set(PRJ_NAME "LogEngineTests")
project(${PRJ_NAME} VERSION 1.0.0 LANGUAGES CXX)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Debug" CACHE STRING "Default build type: Debug" FORCE)
endif()

# Generate and executable.
file(GLOB LTESTS_SRC "*.cpp" )

# Generate the test executable.
add_executable(${PRJ_NAME} ${LTESTS_SRC} )

# Set minimal C++ version to C++20
target_compile_features(${PRJ_NAME} PRIVATE cxx_std_20)

# another way to set C++ standard for target
#set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 20)

# Add include path for cppunit.
target_include_directories(${PRJ_NAME} PRIVATE "${CMAKE_SOURCE_DIR}/external")
#"${CMAKE_SOURCE_DIR}/external"
#"${CMAKE_SOURCE_DIR}/external/libsais/include"
#)

#target_compile_definitions(${PRJ_NAME} PRIVATE WIN32)

target_link_libraries(${PRJ_NAME} PRIVATE 
${CMAKE_SOURCE_DIR}/external/cppunit/lib/libcppunit.a
${CMAKE_SOURCE_DIR}/lib/libLogEngine2.a
)

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    # Add "_DEBUG" definition to the source code.
    target_compile_definitions(${PRJ_NAME} PRIVATE _DEBUG)
else()
    # Add "NDEBUG" definition to the source code.
    target_compile_definitions(${PRJ_NAME} PRIVATE NDEBUG)
endif()

