cmake_minimum_required(VERSION 3.10)
project(CHAT)

set(CMAKE_CXX_STANDARD 17)
include_directories(SYSTEM /usr/include/jsoncpp)

add_subdirectory(main)
add_subdirectory(modules)
add_subdirectory(Utils)
