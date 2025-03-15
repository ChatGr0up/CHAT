cmake_minimum_required(VERSION 3.10)
project(CHAT)

# 设置 vcpkg 作为 CMake 的工具链
set(VCPKG_DIR "${CMAKE_CURRENT_SOURCE_DIR}/depend/vcpkg")             
set(CMAKE_TOOLCHAIN_FILE "${VCPKG_DIR}/scripts/buildsystems/vcpkg.cmake" CACHE STRING "Vcpkg toolchain file")
message(STATUS "CMAKE_TOOLCHAIN_FILE PATH IS: ${VCPKG_DIR}/scripts/buildsystems/vcpkg.cmake")

set(CMAKE_CXX_STANDARD 17)

# 添加 vcpkg 路径到 CMAKE_PREFIX_PATH
list(APPEND CMAKE_PREFIX_PATH "${VCPKG_DIR}/packages")
message(STATUS "CMAKE_PREFIX_PATH: ${CMAKE_PREFIX_PATH}")


# 查找外部依赖,所有的外部依赖都需要现在这里添加，然后才在具体使用它的模块的地方添加
find_package(nlohmann_json CONFIG REQUIRED)

# 添加子目录
add_subdirectory(main)
add_subdirectory(modules)
add_subdirectory(Utils)
