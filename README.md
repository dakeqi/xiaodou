# 小豆 - ESP32-C3 SuperMini 小智AI固件（无LCD版本）

基于 [xiaozhi-esp32](https://github.com/78/xiaozhi-esp32) 官方固件优化，专为 ESP32-C3 SuperMini 开发板设计的**无LCD版本**，降低成本和硬件复杂度。

## 🎯 项目特点

- ✅ **无需LCD屏幕** - 移除了LCD显示功能，节省成本和引脚
- ✅ **语音优先** - 通过LED状态指示和语音反馈实现交互
- ✅ **极简硬件** - 仅需功放+麦克风+按键即可完整使用
- ✅ **低成本** - 硬件成本更低，适合批量制作
- ✅ **引脚释放** - LCD占用的引脚可用于其他扩展

## 📁 项目结构

```
xiaodou/
├── README.md              # 项目文档（完整说明）
├── QUICKSTART.md          # 快速开始指南（新手友好）
├── config.h               # 硬件引脚配置文件
├── xiaodou_board.cc       # 板卡驱动实现（无LCD版本）
├── config.json            # 编译配置文件
├── deploy.sh              # Linux/macOS部署脚本
├── deploy.bat             # Windows部署脚本
├── LICENSE                # MIT开源协议
└── .gitignore             # Git忽略文件
```

## 硬件配置

| 模块 | 型号 | 说明 |
|------|------|------|
| 主控 | ESP32-C3 SuperMini | 4MB Flash / 400KB SRAM |
| 功放 | MAX98357A | I2S数字功放 |
| 麦克风 | INMP441 | I2S数字麦克风 |

## 接线图

### MAX98357A 功放
```
MAX98357A    ESP32-C3
---------    --------
VIN    -->   3.3V
GND    -->   GND
BCLK   -->   GPIO2
LRC    -->   GPIO3
DIN    -->   GPIO4
GAIN   -->   不接(默认9dB) 或 GND(12dB)
SD     -->   不接(默认启用)
```

### INMP441 麦克风
```
INMP441      ESP32-C3
-------      --------
VDD    -->   3.3V
GND    -->   GND
SCK    -->   GPIO5
WS     -->   GPIO6
SD     -->   GPIO7
L/R    -->   GND (左声道)
```

### LED状态指示
```
板载LED      ESP32-C3
-------      --------
LED    -->   GPIO8
```

## 引脚占用总览

| GPIO | 功能 | 模块 |
|------|------|------|
| 2 | BCLK | MAX98357A |
| 3 | LRC | MAX98357A |
| 4 | DIN | MAX98357A |
| 5 | SCK | INMP441 |
| 6 | WS | INMP441 |
| 7 | SD | INMP441 |
| 8 | LED | 板载LED |
| 9 | BOOT | 按键 |

**可用引脚**：GPIO0, GPIO1, GPIO10, GPIO21（原LCD占用的引脚已释放）

## 安装部署

### 方法一：源码编译（推荐）

#### 环境准备

```bash
# 1. 安装 ESP-IDF v5.4+
git clone -b v5.4 --recursive https://github.com/espressif/esp-idf.git
cd esp-idf
./install.sh esp32c3
source export.sh

# 2. 克隆小智官方仓库
git clone --recursive https://github.com/78/xiaozhi-esp32.git
cd xiaozhi-esp32

# 3. 下载小豆板卡配置
git clone https://github.com/dakeqi/xiaodou.git board-config

# 4. 复制板卡文件
mkdir -p main/boards/xiaodou
cp board-config/config.h main/boards/xiaodou/
cp board-config/xiaodou_board.cc main/boards/xiaodou/
```

#### 编译烧录

```bash
# 设置目标芯片
idf.py set-target esp32c3

# 配置（选择 Xiaodou）
idf.py menuconfig
# Xiaozhi Assistant -> Board Type -> Xiaodou (No LCD)

# 编译
idf.py build

# 烧录
idf.py -p COM3 flash

# 查看日志
idf.py -p COM3 monitor
```

### 方法二：预编译固件烧录

访问 [Releases](https://github.com/dakeqi/xiaodou/releases) 下载最新固件，使用 [ESP Flash Download Tool](https://www.espressif.com/zh-hans/support/download/other-tools) 烧录。

烧录地址：
- `bootloader.bin` -> `0x0`
- `partition-table.bin` -> `0x8000`
- `xiaodou.bin` -> `0x10000`

## 使用说明

### 首次配网

1. **上电启动** - 设备通电后进入配网模式
2. **LED指示** - LED快速闪烁表示等待配网
3. **扫码配网** - 使用小智APP扫码配网
4. **连接成功** - LED常亮表示WiFi连接成功

### 语音交互

- **唤醒词**：「你好小智」
- **LED状态**：
  - 快速闪烁：配网模式
  - 常亮：待机状态
  - 呼吸灯：对话中
  - 闪烁：处理中

### 按键操作

| 操作 | 功能 |
|------|------|
| 短按 BOOT | 手动唤醒/停止对话 |
| 长按 BOOT 5秒 | 重置WiFi配置 |

## 与原版区别

| 特性 | 原版（带LCD） | 小豆（无LCD） |
|------|--------------|--------------|
| 显示屏 | ST7789 240x240 | ❌ 无 |
| 配网方式 | 屏幕显示二维码 | APP扫描/串口获取 |
| 状态反馈 | 屏幕显示 | LED指示灯 |
| 占用引脚 | 10个 | 6个 |
| 硬件成本 | 较高 | 较低 |
| 可用引脚 | 少 | 多（可扩展） |

## 调试指南

### 串口日志

```bash
# Windows
idf.py -p COM3 monitor

# Linux/Mac
idf.py -p /dev/ttyUSB0 monitor
```

### 常见问题

#### 1. 没有声音
- 检查 MAX98357A 接线是否正确
- 确认 BCLK/LRC/DIN 三根线都已连接
- 检查扬声器是否正常（4Ω-8Ω）

#### 2. 麦克风无法录音
- 检查 INMP441 的 L/R 引脚是否接 GND
- 确认 SCK/WS/SD 接线正确
- 检查 VDD 是否为 3.3V

#### 3. 无法唤醒
- 确保环境安静，靠近麦克风说话
- 标准普通话发音："你-好-小-智"
- 检查串口日志是否有识别信息

#### 4. WiFi连接失败
- 确认WiFi为2.4GHz（不支持5GHz）
- 检查密码是否正确
- 长按BOOT 5秒重新配网

#### 5. 如何配网（无LCD版本）
- 方式1：使用小智APP扫描设备
- 方式2：通过串口日志查看配网信息
- 方式3：使用 `idf.py monitor` 查看配网URL

## 扩展功能

释放的引脚可用于：
- 添加传感器（温湿度、光照等）
- 添加更多按键
- 添加继电器控制
- SPI/I2C设备扩展

## 相关链接

- [小智官方固件](https://github.com/78/xiaozhi-esp32)
- [小智官方服务](https://xiaozhi.me)
- [原版固件（带LCD）](https://github.com/dakeqi/xiaozhi-esp32c3-supermini)
- [ESP32-C3 SuperMini 资料](https://www.cnblogs.com/wuqiyang/p/18932737)

## 贡献

欢迎提交 Issue 和 Pull Request！

## License

MIT License
