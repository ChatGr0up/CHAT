export CHAT_ROOT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
export GLOBAL_CONFIG_PATH=$CHAT_ROOT_DIR/configs
export LOG_DIR=$CHAT_ROOT_DIR
export LIB_PATH=$CHAT_ROOT_DIR/build/lib
export BUILD_TYPE=$CHAT_ROOT_DIR

echo "GLOBAL_CONFIG_PATH is "$GLOBAL_CONFIG_PATH""
echo "LOG_DIR is "$LOG_DIR""
echo "BUILD_TYPE is "$BUILD_TYPE""

echo "Now start the MicroService!"
./build/main/main