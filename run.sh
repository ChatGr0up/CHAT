export CHAT_ROOT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
export GLOBAL_CONFIG_PATH=$CHAT_ROOT_DIR/configs
export LOG_DIR=$CHAT_ROOT_DIR
export LIB_PATH=$CHAT_ROOT_DIR/build/lib
export BUILD_TYPE=$CHAT_ROOT_DIR

echo "GLOBAL_CONFIG_PATH is "$GLOBAL_CONFIG_PATH""
echo "LOG_DIR is "$LOG_DIR""
echo "BUILD_TYPE is "$BUILD_TYPE""

echo "Now kill the last MicroService!"
ps -ef | grep "./build/main/main" | grep -v grep | awk '{print $2}' | xargs kill -9

echo "Now start the MicroService!"

if [[ "$1" == "debug" ]]; then
    echo "[DEBUG MODE] Running with strace..."
    strace -o "$LOG_DIR/strace_output.log" ./build/main/main > "$LOG_DIR/main.log" 2>&1 &
elif [[ "$1" == "debug-gdb" ]]; then
    echo "[DEBUG MODE] Running with GDB..."
    gdb --args ./build/main/main
else
    echo "[RELEASE MODE] Running with nohup..."
    nohup ./build/main/main > "$LOG_DIR/main.log" 2>&1 &
fi