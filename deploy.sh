#!/bin/bash

# 小豆固件部署脚本
# 用于将小豆板卡配置集成到小智ESP32项目中

set -e

echo "=========================================="
echo "  小豆固件部署脚本"
echo "  无LCD版本 - ESP32-C3 SuperMini"
echo "=========================================="
echo ""

# 检查是否在小智ESP32项目根目录
if [ ! -d "main" ] || [ ! -f "CMakeLists.txt" ]; then
    echo "❌ 错误: 请在小智ESP32项目根目录下运行此脚本"
    echo "   示例: cd xiaozhi-esp32 && bash deploy.sh"
    exit 1
fi

echo "✓ 检测到小智ESP32项目"
echo ""

# 创建板卡目录
BOARD_DIR="main/boards/xiaodou"
echo "📁 创建板卡目录: $BOARD_DIR"
mkdir -p "$BOARD_DIR"

# 复制配置文件
echo "📋 复制配置文件..."
if [ -f "board-config/config.h" ]; then
    cp board-config/config.h "$BOARD_DIR/"
    echo "  ✓ config.h"
else
    echo "  ❌ 找不到 config.h"
    exit 1
fi

if [ -f "board-config/xiaodou_board.cc" ]; then
    cp board-config/xiaodou_board.cc "$BOARD_DIR/"
    echo "  ✓ xiaodou_board.cc"
else
    echo "  ❌ 找不到 xiaodou_board.cc"
    exit 1
fi

echo ""
echo "✅ 部署完成！"
echo ""
echo "接下来的步骤:"
echo "1. 设置目标芯片: idf.py set-target esp32c3"
echo "2. 配置项目: idf.py menuconfig"
echo "   → Xiaozhi Assistant -> Board Type -> Xiaodou (No LCD)"
echo "3. 编译: idf.py build"
echo "4. 烧录: idf.py -p COM3 flash"
echo "5. 监控: idf.py -p COM3 monitor"
echo ""
echo "详细文档请查看: https://github.com/dakeqi/xiaodou"
echo ""
