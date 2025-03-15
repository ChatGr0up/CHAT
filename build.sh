#!/bin/bash
set -e  # 遇到错误立即退出

# 获取当前脚本所在的目录（即 CHAT 根目录）
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"

# 1. 运行依赖管理脚本
echo "Running dependency installation..."
bash "$SCRIPT_DIR/scripts/dependencies/dependencies.sh"

# 2. 运行 CMake 编译
BUILD_DIR="$SCRIPT_DIR/build"
echo "Running CMake build..."
cmake -B "$BUILD_DIR" -DCMAKE_TOOLCHAIN_FILE="$SCRIPT_DIR/depend/vcpkg/scripts/buildsystems/vcpkg.cmake"
cmake --build "$BUILD_DIR"

echo "Build complete!"
