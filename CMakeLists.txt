cmake_minimum_required (VERSION 3.20)

# Enable Hot Reload for MSVC compilers if supported.
#if (POLICY CMP0141)
#  cmake_policy(SET CMP0141 NEW)
#  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
#endif()

if(POLICY CMP0091)
    cmake_policy(SET CMP0091 NEW)
endif()

if(NOT CMAKE_CONFIGURATION_TYPES AND NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug CACHE STRING "Build type")
endif()

#set(CMAKE_C_COMPILER gcc-13) // we dont use C compiler
set(CMAKE_CXX_COMPILER g++-13)

set(LOGENGINE_VER "1.06")
set(PROJECT_NAME "LogEngine2GlobalCMake")
project(${PROJECT_NAME} VERSION ${LOGENGINE_VER} LANGUAGES CXX)

if(MSVC)
    set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>" CACHE STRING "MSVC runtime library")
endif()

message(STATUS "Binary directory: ${CMAKE_BINARY_DIR}")
message(STATUS "Build Type: ${CMAKE_BUILD_TYPE}")

# configure variables where output files are stored
#set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib) 
#set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
#set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_BINARY_DIR}/rbin)
#set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_BINARY_DIR}/dbin)


#сораняем путь до директории include нашими хедарами 
#set(PROG_INCLUDE "${CMAKE_CURRENT_SOURCE_DIR}/include")

#добавляет путь до нашей директории в область видимости всех проектов
#include_directories(${PROG_INCLUDE})

#добавляем директории наших подпроектов в область видимости CMake 
add_subdirectory("src")
add_subdirectory("test")
add_subdirectory("example")
