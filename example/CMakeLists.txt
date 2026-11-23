# CMake project for LogEngine2 sample application, 
#

set(EXAMPLE_APP "LogEngine2Example")
#project(${EXAMPLE_APP} VERSION ${LOGENGINE_VER} LANGUAGES CXX)

# CONFIGURE_DEPENDS tells cmake to re-build list of files each time you run 'cmake --build build'
file(GLOB EXAMPLE_SRC CONFIGURE_DEPENDS "*.cpp")

set(target ${EXAMPLE_APP})
add_executable(${target} ${EXAMPLE_SRC})
set_target_properties(${target} PROPERTIES OUTPUT_NAME "Example")

# used cxx_std_20 from LogEngine2 target that Example app depens on.
#target_compile_features(${EXAMPLE_APP} PRIVATE cxx_std_20)

target_link_libraries(${EXAMPLE_APP} PRIVATE LogEngine2)

#target_link_libraries(${target} ${LOGENGINE_LIB})
#target_link_libraries(${target} version.lib)



