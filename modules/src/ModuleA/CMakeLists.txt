# modules/moduleA/CMakeLists.txt
include(${CMAKE_SOURCE_DIR}/cmake/module.cmake)

include_directories(${CMAKE_SOURCE_DIR}/modules/interface)
include_directories(${CMAKE_SOURCE_DIR}/Utils)

add_module(ModuleA DEPENDS ModuleAItf DataBaseManager)
