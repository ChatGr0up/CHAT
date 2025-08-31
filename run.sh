#!/bin/bash

set -e  # 一旦出错就退出脚本

# 颜色输出
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
NC='\033[0m'

log() {
    echo -e "${YELLOW}[$(date +%T)] $1${NC}"
}

# ----------- 参数解析 -----------
MODE="dev"
DEBUG_MODE="none"

for arg in "$@"; do
    case $arg in
        --mode=prod)
            MODE="prod"
            ;;
        --mode=dev)
            MODE="dev"
            ;;
        debug)
            DEBUG_MODE="strace"
            ;;
        debug-gdb)
            DEBUG_MODE="gdb"
            ;;
        *)
            log "未知参数: $arg"
            ;;
    esac
done

export CADDY_ENV="$MODE"
log "当前环境: $CADDY_ENV"
log "调试模式: $DEBUG_MODE"

# ----------- 路径设置 -----------
export CHAT_ROOT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
export GLOBAL_CONFIG_PATH="$CHAT_ROOT_DIR/configs"
export LOG_DIR="$CHAT_ROOT_DIR"
export LIB_PATH="$CHAT_ROOT_DIR/build/lib"
export BUILD_TYPE="$CHAT_ROOT_DIR"
export GLOBAL_REST_CONFIG_PATH="$GLOBAL_CONFIG_PATH"
export SSL_PEM_PATH="$HOME"
export REDIS_CONFIG_PATH="$GLOBAL_CONFIG_PATH"
export LD_LIBRARY_PATH=/home/liu/code/CHAT/build/lib:/home/liu/code/CHAT/thirdPartyDepends/lib:$LD_LIBRARY_PATH

# ----------- 渲染 Drogon 配置 -----------
TEMPLATE_CONFIG="$GLOBAL_CONFIG_PATH/drogon_config_template.json"
FINAL_CONFIG="$GLOBAL_CONFIG_PATH/DrogonRestConfig.json"
sed "s|\$SSL_PEM_PATH|$SSL_PEM_PATH|g" "$TEMPLATE_CONFIG" > "$FINAL_CONFIG"

# ----------- 生成 SSL 证书 -----------
log "生成 SSL 证书中..."
chmod +x "$CHAT_ROOT_DIR/scripts/ssl/make_cert.sh"
"$CHAT_ROOT_DIR/scripts/ssl/make_cert.sh"

# ----------- 打印路径信息 -----------
log "GLOBAL_CONFIG_PATH=$GLOBAL_CONFIG_PATH"
log "LOG_DIR=$LOG_DIR"
log "BUILD_TYPE=$BUILD_TYPE"

# ----------- 杀死旧的 MicroService -----------
log "杀死旧 MicroService..."
pids=$(pgrep -f "./build/main/chat" || true)
if [[ -n "$pids" ]]; then
    echo "$pids" | xargs kill -9
    log "已杀死进程: $pids"
else
    log "未发现旧进程。"
fi

# ----------- 启动 MicroService -----------
log "启动新的 MicroService..."
case "$DEBUG_MODE" in
    strace)
        log "[DEBUG] 使用 strace 启动"
        strace -o "$LOG_DIR/strace_output.log" ./build/main/chat > "$LOG_DIR/main.log" 2>&1 &
        ;;
    gdb)
        log "[DEBUG] 使用 gdb 启动"
        gdb --args ./build/main/chat
        ;;
    *)
        log "[RELEASE] 使用 nohup 启动"
        nohup ./build/main/chat > "$LOG_DIR/main.log" 2>&1 &
        ;;
esac

# ----------- 启动 Caddy -----------
log "启动 Caddy（环境: $CADDY_ENV）..."
CADDY_CONFIG="$CHAT_ROOT_DIR/configs/${CADDY_ENV}.caddy"
log "使用配置文件：$CADDY_CONFIG"
sudo pkill -f caddy
if [[ "$CADDY_ENV" == "prod" ]]; then
    sudo caddy start --config "$CADDY_CONFIG" --adapter caddyfile
else
    sudo caddy start --config "$CADDY_CONFIG" --adapter caddyfile
fi

log "${GREEN}启动完成！${NC}"