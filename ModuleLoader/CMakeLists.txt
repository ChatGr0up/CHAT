project(ModuleLoader)

set(EXT_LINK_LIB ModuleBase)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../ModuleBase)

include(${CMAKE_CURRENT_SOURCE_DIR}/../cmake/module.cmake)
