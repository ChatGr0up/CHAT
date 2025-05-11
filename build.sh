#!/bin/bash
set -ueo pipefail

ROOT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"

echo "Project root path is: "$ROOT_DIR""

BUILD_DIR="$ROOT_DIR/build"
echo "Running CMake build..."

cmake -B "$BUILD_DIR" -S "$ROOT_DIR" \
  -DCMAKE_BUILD_TYPE=Debug \
  -DMYSQL_INCLUDE_DIRS=/usr/include/mysql \
  -DMYSQL_LIBRARY=/usr/lib/aarch64-linux-gnu/libmysqlclient.so \
  -DMYSQL_LIBRARIES=/usr/lib/aarch64-linux-gnu/libmysqlclient.so

cmake --build "$BUILD_DIR"

echo "Build complete! Successfully build the CHAT project"
