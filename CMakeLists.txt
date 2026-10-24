cmake_minimum_required (VERSION 3.8)

# Enable Hot Reload for MSVC compilers if supported.
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

set(CMAKE_C_COMPILER gcc-13)
set(CMAKE_CXX_COMPILER g++-13)

#настраиваем глобальные стандарты языка
#set(CMAKE_CXX_STANDARD 20)
#set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(PROJECT_NAME "LogEngine2GlobalCMake")
project(${PROJECT_NAME} VERSION 1.0.5 LANGUAGES CXX)

#set(EXAMPLE_APP "LogEngine2Example")

message(STATUS "Binary directory: ${CMAKE_BINARY_DIR}")

#настраиваем места хранения скомпилированных файлов
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib) 
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_BINARY_DIR}/rbin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_BINARY_DIR}/dbin)


#сораняем путь до директории include нашими хедарами 
set(PROG_INCLUDE "${CMAKE_CURRENT_SOURCE_DIR}/include")

#добавляет путь до нашей директории в область видимости всех проектов
include_directories(${PROG_INCLUDE})

#добавляем директории наших подпроектов в область видимости CMake 
add_subdirectory("src")
add_subdirectory("test")

