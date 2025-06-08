#!/bin/bash

set -e

INSTALL_DIR="/usr/local/redis/redis-plus-plus"
SRC_DIR="$HOME/temp"
REDISPP_DIR="$SRC_DIR/redis-plus-plus"
HIREDIS_DIR="$SRC_DIR/hiredis"
HIREDIS_INSTALL_DIR="/usr/local/redis/hiredis"

install_hiredis() {
  if [ -f "$HIREDIS_INSTALL_DIR/lib/libhiredis.so" ]; then
    echo "hiredis 已安装在 $HIREDIS_INSTALL_DIR，跳过安装。"
    return
  fi

  echo "开始克隆 hiredis 仓库..."
  cd "$SRC_DIR"
  if [ ! -d "$HIREDIS_DIR" ]; then
    git clone https://github.com/redis/hiredis.git
  else
    echo "hiredis 仓库已存在，跳过克隆。"
  fi

  echo "开始构建 hiredis..."
  cd "$HIREDIS_DIR"
  mkdir -p build && cd build
  cmake .. -DCMAKE_INSTALL_PREFIX="$HIREDIS_INSTALL_DIR" \
           -DENABLE_SSL=ON
  make -j$(nproc)
  sudo make install

  echo "hiredis 安装完成！"
}

install_redispp() {
  if [ -f "$INSTALL_DIR/lib/libredis++.so" ]; then
    echo "redis-plus-plus 已经安装在 $INSTALL_DIR, 跳过构建！"
    exit 0
  fi

  mkdir -p "$SRC_DIR"
  cd "$SRC_DIR"

  # 检查 redis++ 是否已存在
  if [ ! -d "$REDISPP_DIR" ]; then
    echo "开始克隆 redis-plus-plus 仓库..."
    git clone https://github.com/sewenew/redis-plus-plus.git
  else
    echo "redis-plus-plus 仓库已存在，跳过克隆！"
  fi

  cd "$REDISPP_DIR"
  mkdir -p build && cd build

  echo "开始构建 redis-plus-plus 的 CMake"

  cmake .. \
    -DCMAKE_INSTALL_PREFIX="$INSTALL_DIR" \
    -DREDIS_PLUS_PLUS_BUILD_TEST=OFF \
    -DREDIS_PLUS_PLUS_BUILD_STATIC=OFF \
    -DREDIS_PLUS_PLUS_USE_TLS=ON \
    -DCMAKE_PREFIX_PATH="$HIREDIS_INSTALL_DIR"

  echo "开始编译 redis-plus-plus..."
  make -j$(nproc)

  echo "安装 redis-plus-plus 到 $INSTALL_DIR..."
  sudo make install
}

install_hiredis
install_redispp


echo "redis-plus-plus 安装完成！"