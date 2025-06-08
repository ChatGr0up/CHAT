include(${CMAKE_SOURCE_DIR}/cmake/module.cmake)

include_directories(${CMAKE_SOURCE_DIR}/Utils)

find_package(Drogon REQUIRED)
link_directories(/usr/lib/aarch64-linux-gnu)
add_module(RestFrame Depends JsonUtils Logger EnvManager LIBS Drogon::Drogon hiredis)