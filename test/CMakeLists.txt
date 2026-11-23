
set(LETESTS_APP "LogEngineTests")
#project(${PRJ_NAME} VERSION ${LOGENGINE_VER} LANGUAGES CXX)

#if(NOT CMAKE_BUILD_TYPE)
#    set(CMAKE_BUILD_TYPE "Debug" CACHE STRING "Default build type: Debug" FORCE)
#endif()

# CONFIGURE_DEPENDS tells cmake to re-build list of files each time you run 'cmake --build build'
file(GLOB LTESTS_SRC CONFIGURE_DEPENDS "*.cpp" )

# Generate the test executable.
add_executable(${LETESTS_APP} ${LTESTS_SRC} )

# LETests depends on LogEgnine2 target, that is why cxx_std_20 from LogEngine2 applies to LETests too (see CMakeLists.txt for LogEgnine2)
# it means that we may remove line below.
target_compile_features(${LETESTS_APP} PRIVATE cxx_std_20)

# another way to set C++ standard for target
#set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 20)

# Add include path for cppunit.
target_include_directories(${LETESTS_APP} PRIVATE "${CMAKE_SOURCE_DIR}/external")

#target_compile_definitions(${PRJ_NAME} PRIVATE WIN32)

if(MSVC)
    add_library(CppUnit STATIC IMPORTED)

    set_target_properties(CppUnit PROPERTIES 
        IMPORTED_CONFIGURATIONS "DEBUG;RELEASE"

        IMPORTED_LOCATION_DEBUG "${CMAKE_SOURCE_DIR}/external/cppunit/lib/cppunitd64.lib"
        IMPORTED_LOCATION_RELEASE "${CMAKE_SOURCE_DIR}/external/cppunit/lib/cppunit64.lib"

        MAP_IMPORTED_CONFIG_RELWITHDEBINFO Release
        MAP_IMPORTED_CONFIG_MINSIZEREL Release

        INTERFACE_INCLUDE_DIRECTORIES "${CMAKE_SOURCE_DIR}/external"
    )
else()
    add_library(CppUnit STATIC IMPORTED)

    set_target_properties(CppUnit PROPERTIES
        IMPORTED_LOCATION "${CMAKE_SOURCE_DIR}/external/cppunit/lib/libcppunit.a"

        INTERFACE_INCLUDE_DIRECTORIES "${CMAKE_SOURCE_DIR}/external"
    )
endif()

target_link_libraries(${LETESTS_APP} PRIVATE LogEngine2 CppUnit)

#if(MSVC)
#    target_link_libraries(${LETESTS_APP} PRIVATE "${CMAKE_SOURCE_DIR}/external/cppunit/lib/cppunit64.lib")
#else()
#    target_link_libraries(${LETESTS_APP} PRIVATE "${CMAKE_SOURCE_DIR}/external/cppunit/lib/libcppunit.a")
#endif()

target_compile_options(${LETESTS_APP} PRIVATE
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

target_compile_definitions(${LETESTS_APP} PRIVATE
    $<$<CONFIG:Debug>:_DEBUG>
    $<$<CONFIG:Release>:NDEBUG>
)

#if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    # Add "_DEBUG" definition to the source code.
#    target_compile_definitions(${PRJ_NAME} PRIVATE _DEBUG)
#else()
    # Add "NDEBUG" definition to the source code.
#    target_compile_definitions(${PRJ_NAME} PRIVATE NDEBUG)
#endif()

