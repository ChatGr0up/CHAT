#!/bin/bash
set -e  # 如果任何命令失败，脚本会立即退出

echo "更新系统包..."
sudo apt update
sudo apt upgrade -y

echo "安装必要的开发工具..."
sudo apt install -y build-essential cmake

echo "安装 JSON 解析库..."
sudo apt install -y libjsoncpp-dev

echo "安装 MySQL 服务器和开发库..."
sudo apt install -y mysql-server libmysqlclient-dev

echo "所有依赖安装完成！"
