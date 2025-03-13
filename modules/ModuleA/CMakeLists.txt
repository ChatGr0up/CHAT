# modules/moduleA/CMakeLists.txt
include(${CMAKE_SOURCE_DIR}/cmake/module.cmake)
add_module(moduleA moduleA.cpp)
