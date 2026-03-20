#!/bin/bash

# AT Command Test Script for ESP8266 Terminal Display
# 使用方法: ./test_at.sh [次数]

PORT="/dev/ttyCH341USB0"
BAUD=115200
INTERVAL=2

# 测试消息列表
MESSAGES=(
    "HELLO WORLD"
    "ESP8266 READY"
    "MINIMAX-M2.5"
    "TERMINAL MODE"
    "TEST MESSAGE"
    "RANDOM DATA"
    "1234567890"
    "ABCDEFGHIJ"
    "OPENCODE AI"
    "EMBEDDED SYSTEM"
)

# 清屏
clear_screen() {
    printf "AT+CLEAR\r\n" > "$PORT"
    sleep 0.3
}

# 发送文本
send_text() {
    local msg="$1"
    printf "AT+TEXT=%s\r\n" "$msg" > "$PORT"
    echo "Sent: $msg"
}

# 随机消息
random_text() {
    local idx=$((RANDOM % ${#MESSAGES[@]}))
    echo "${MESSAGES[$idx]}"
}

# 主测试
test_loop() {
    local count="${1:-10}"
    local i=0
    
    echo "Starting AT Command Test..."
    echo "Port: $PORT, Baud: $BAUD"
    echo "Press Ctrl+C to stop"
    echo ""
    
    while [ $i -lt $count ]; do
        msg=$(random_text)
        send_text "$msg"
        sleep "$INTERVAL"
        ((i++))
    done
    
    echo "Test completed: $i messages sent"
}

# 显示帮助
show_help() {
    echo "Usage: $0 [options]"
    echo ""
    echo "Options:"
    echo "  -n, --count N     测试次数 (默认: 10)"
    echo "  -i, --interval N  间隔秒数 (默认: 2)"
    echo "  -c, --clear       仅清屏"
    echo "  -s, --show        显示当前内容"
    echo "  -h, --help        显示帮助"
    echo ""
    echo "Examples:"
    echo "  $0 -n 5           测试5次"
    echo "  $0 -c             清屏"
    echo "  $0 -s             显示当前内容"
}

# 解析参数
case "${1:-}" in
    -n|--count)
        test_loop "$2"
        ;;
    -i|--interval)
        INTERVAL="$2"
        test_loop 10
        ;;
    -c|--clear)
        clear_screen
        echo "Screen cleared"
        ;;
    -s|--show)
        printf "AT+SHOW\r\n" > "$PORT"
        sleep 0.5
        cat "$PORT"
        ;;
    -h|--help)
        show_help
        ;;
    "")
        test_loop 10
        ;;
    *)
        send_text "$1"
        ;;
esac
