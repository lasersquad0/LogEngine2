# CMake project for LogEngine2 library
#

cmake_minimum_required (VERSION 3.20)

set(LOGENGINE_LIB "LogEngine2")

# CONFIGURE_DEPENDS tells cmake to re-build list of files each time you run 'cmake --build build' 
file(GLOB INCLUDE_FILES CONFIGURE_DEPENDS "*.h")
file(GLOB SRC_FILES CONFIGURE_DEPENDS "*.cpp")

set(target ${LOGENGINE_LIB})

add_library(${target} STATIC ${SRC_FILES} ${INCLUDE_FILES} )

target_include_directories(${LOGENGINE_LIB} PUBLIC "${CMAKE_SOURCE_DIR}/include")

# Set minimal C++ version to C++20, dependent targets will also have cxx_std_20 set (because of PUBLIC)
target_compile_features(${LOGENGINE_LIB} PUBLIC cxx_std_20)

target_compile_options(LogEngine2 PRIVATE
    $<$<CXX_COMPILER_ID:GNU,Clang>:
        -Wall
        -Wextra
        -Wpedantic
    >
    $<$<CXX_COMPILER_ID:MSVC>:
        /W4
        /permissive-
        /EHsc
    >
)

target_compile_definitions(LogEngine2 PRIVATE
    $<$<CONFIG:Debug>:_DEBUG>
    $<$<CONFIG:Release>:NDEBUG>
)




