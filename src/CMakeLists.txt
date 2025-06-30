# CMakeList.txt : CMake project for CMakeProject, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

#set(LOGENGINE_LIB "LogEngine2Lib")
project(${LOGENGINE_LIB} LANGUAGES CXX)

file(GLOB includevar "*.h")      #команда для работы с файлами
file(GLOB srcvar "*.cpp")

set(target ${LOGENGINE_LIB})

add_library(${target} STATIC ${srcvar} ${includevar} )



