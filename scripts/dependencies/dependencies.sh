#!/bin/bash
set -e  # 遇到错误立即退出

# 计算项目根目录
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
CHAT_ROOT="$(cd "$SCRIPT_DIR/../.." && pwd)"   # 回到 CHAT 目录
DEPEND_DIR="$CHAT_ROOT/depend"                 # 依赖存放目录
VCPKG_DIR="$DEPEND_DIR/vcpkg"                  # vcpkg 安装目录
DEPS_FILE="$CHAT_ROOT/configs/dependencies.txt"

# 确保依赖目录存在
mkdir -p "$DEPEND_DIR"

# 0. 确保必要的工具已安装
echo "Checking required tools..."
if ! command -v zip &>/dev/null || ! command -v unzip &>/dev/null || ! command -v tar &>/dev/null; then
    echo "Installing required tools..."
    sudo apt-get update && sudo apt-get install -y curl zip unzip tar
else
    echo "All required tools are already installed."
fi

# 1. 检查 vcpkg 是否安装
if [ ! -d "$VCPKG_DIR" ]; then
    echo "Installing vcpkg..."
    git clone git@github.com:microsoft/vcpkg.git "$VCPKG_DIR"
    "$VCPKG_DIR/bootstrap-vcpkg.sh"
else 
    echo "vcpkg already installed." 
    if [ ! -f "$VCPKG_DIR/vcpkg" ]; then
        echo "Bootstrapping vcpkg..."
        "$VCPKG_DIR/bootstrap-vcpkg.sh"
    fi   
fi

# 2. 读取 dependencies.txt 并安装依赖
if [ -f "$DEPS_FILE" ]; then
    grep -v '^#' "$DEPS_FILE" | while IFS= read -r dep || [[ -n "$dep" ]]; do
        echo "Installing dependency: $dep"
        "$VCPKG_DIR/vcpkg" install "$dep"
    done
else
    echo "No dependencies file found at $DEPS_FILE!"
    exit 1
fi

echo "All dependencies installed."
