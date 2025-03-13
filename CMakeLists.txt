cmake_minimum_required(VERSION 3.10)
project(CHAT)

set(CMAKE_CXX_STANDARD 17)

# 指定模块路径
add_subdirectory(main)
add_subdirectory(modules/ModuleA)
add_subdirectory(modules/ModuleB)
