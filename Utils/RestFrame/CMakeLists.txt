include(${CMAKE_SOURCE_DIR}/cmake/module.cmake)

include_directories(${CMAKE_SOURCE_DIR}/Utils)

find_package(Drogon REQUIRED)

add_module(RestFrame Depends JsonUtils Logger EnvManager LIBS Drogon::Drogon)