include(${CMAKE_SOURCE_DIR}/cmake/module.cmake)
include_directories(${CMAKE_SOURCE_DIR}/Utils)

find_package(Drogon REQUIRED)

add_executable(main main.cpp)

target_link_libraries(main dl Module JsonUtils EnvManager Logger Thread RestFrame Drogon::Drogon)