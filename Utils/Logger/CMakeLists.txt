include(${CMAKE_SOURCE_DIR}/cmake/module.cmake)

include_directories(${CMAKE_SOURCE_DIR}/Utils)

add_module(Logger DEPENDS EnvManager JsonUtils FileUtils Thread TimeUtils)