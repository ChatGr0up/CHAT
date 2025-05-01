# modules/moduleA/CMakeLists.txt
include(${CMAKE_SOURCE_DIR}/cmake/module.cmake)

include_directories(${CMAKE_SOURCE_DIR}/Utils)
include_directories(${CMAKE_SOURCE_DIR}/modules/interface)

add_module(ModuleB DEPENDS ModuleBItf ModuleAItf)
