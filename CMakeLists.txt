cmake_minimum_required(VERSION 3.17)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 指定所有的子模块
set(SUBDIRS ModuleLoader ModuleBase)

# 引入 include.cmake 来处理子目录的遍历和构建
include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/include.cmake)

# 遍历所有子目录并添加到构建中
foreach(SUBDIR ${SUBDIRS})
    add_subdirectory(${SUBDIR})
endforeach()
