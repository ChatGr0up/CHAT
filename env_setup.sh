#!/bin/bash
set -e  # 如果任何命令失败，脚本会立即退出

echo "开始拉取构建CHAT所需的全部依赖!"

echo "更新系统包..."
sudo apt update
sudo apt upgrade -y

echo "安装必要的开发工具..."
sudo apt install -y build-essential=12.10ubuntu1 cmake=3.28.3-1build7

echo "安装 JSON 解析库..."
sudo apt install -y libjsoncpp-dev=1.9.5-6build1

echo "安装 MySQL 服务器和开发库..."
sudo apt install -y libmysqlclient-dev=8.0.42-0ubuntu0.24.04.1

echo "安装drogon依赖..."
sudo apt install uuid-dev=2.39.3-9ubuntu6.2 \
    libpq-dev=16.9-0ubuntu0.24.04.1 \
    libsqlite3-dev=3.45.1-1ubuntu2.3 \
    libhiredis-dev=1.2.0-6ubuntu3 \
    libyaml-cpp-dev=0.8.0+dfsg-6build1 \
    libdrogon-dev=1.8.7+ds-1.1build1 \
    librdkafka-dev=2.3.0-1build2 \
    caddy=2.6.2-6ubuntu0.24.04.2 \

install_redis_dependency() {
  echo "安装 Redis 依赖..."

  local script_path="./scripts/thirdpartyLib/redis.sh"

  if [ ! -x "$script_path" ]; then
    echo "赋予 redis.sh 执行权限..."
    chmod +x "$script_path"
  fi

  if ! "$script_path"; then
    echo "redis.sh 执行失败，请检查脚本内容！"
    exit 1
  fi
}

install_redis_dependency

echo "所有依赖安装完成！"
