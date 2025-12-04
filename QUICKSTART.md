# 快速开始指南

本指南将帮助您快速构建和部署小豆固件到ESP32-C3 SuperMini开发板。

## 前置要求

### 硬件
- ESP32-C3 SuperMini 开发板 × 1
- MAX98357A I2S数字功放模块 × 1
- INMP441 I2S数字麦克风模块 × 1
- 4Ω-8Ω扬声器 × 1
- USB Type-C数据线 × 1
- 杜邦线若干

### 软件
- ESP-IDF v5.4 或更高版本
- Git
- Python 3.8+

## 步骤1：安装ESP-IDF

### Windows
```bash
# 下载ESP-IDF安装器
# 访问: https://dl.espressif.com/dl/esp-idf/

# 或使用命令行
git clone -b v5.4 --recursive https://github.com/espressif/esp-idf.git
cd esp-idf
install.bat esp32c3
export.bat
```

### Linux/macOS
```bash
git clone -b v5.4 --recursive https://github.com/espressif/esp-idf.git
cd esp-idf
./install.sh esp32c3
source export.sh
```

## 步骤2：克隆项目

```bash
# 克隆小智ESP32官方仓库
git clone --recursive https://github.com/78/xiaozhi-esp32.git
cd xiaozhi-esp32

# 克隆小豆配置仓库
git clone https://github.com/dakeqi/xiaodou.git board-config
```

## 步骤3：部署配置

### Windows
```bash
board-config\deploy.bat
```

### Linux/macOS
```bash
bash board-config/deploy.sh
```

或者手动复制：
```bash
mkdir -p main/boards/xiaodou
cp board-config/config.h main/boards/xiaodou/
cp board-config/xiaodou_board.cc main/boards/xiaodou/
```

## 步骤4：硬件连接

### MAX98357A 功放
| MAX98357A | ESP32-C3 |
|-----------|----------|
| VIN | 3.3V |
| GND | GND |
| BCLK | GPIO2 |
| LRC | GPIO3 |
| DIN | GPIO4 |

### INMP441 麦克风
| INMP441 | ESP32-C3 |
|---------|----------|
| VDD | 3.3V |
| GND | GND |
| SCK | GPIO5 |
| WS | GPIO6 |
| SD | GPIO7 |
| L/R | GND |

## 步骤5：编译固件

```bash
# 设置目标芯片
idf.py set-target esp32c3

# 配置项目
idf.py menuconfig
# 在菜单中选择:
# Xiaozhi Assistant -> Board Type -> Xiaodou (No LCD)

# 编译
idf.py build
```

## 步骤6：烧录固件

```bash
# Windows
idf.py -p COM3 flash

# Linux/macOS
idf.py -p /dev/ttyUSB0 flash
```

> 💡 提示：如果烧录失败，按住BOOT键后重新执行烧录命令

## 步骤7：查看日志

```bash
# Windows
idf.py -p COM3 monitor

# Linux/macOS
idf.py -p /dev/ttyUSB0 monitor
```

退出监控：`Ctrl + ]`

## 步骤8：配网

设备首次启动时会进入配网模式：

1. **查看配网信息**
   - 通过串口日志可以看到配网URL或二维码信息
   
2. **使用小智APP配网**
   - 下载小智APP
   - 扫描设备或手动添加
   - 输入WiFi名称和密码

3. **通过串口监控**
   - 在串口日志中会显示配网URL
   - 可以通过浏览器访问该URL进行配网

## 步骤9：开始使用

配网成功后：
- LED常亮表示待机
- 说出唤醒词："你好小智"
- LED呼吸灯表示正在对话
- 开始语音交互

## 常见问题排查

### 编译错误

**错误**: `Board type not found`
**解决**: 确认已正确复制配置文件到 `main/boards/xiaodou/`

**错误**: `Missing dependencies`
**解决**: 执行 `git submodule update --init --recursive`

### 烧录问题

**问题**: 无法识别串口
**解决**: 
1. 安装CH343驱动（如需要）
2. 检查USB线是否支持数据传输
3. 按住BOOT键再连接USB

**问题**: 烧录超时
**解决**: 降低波特率 `idf.py -p COM3 -b 115200 flash`

### 运行问题

**问题**: 没有声音
**解决**:
1. 检查MAX98357A接线
2. 检查扬声器连接
3. 查看串口日志是否有错误

**问题**: 无法录音
**解决**:
1. 确认INMP441的L/R引脚接GND
2. 检查VDD是否为3.3V
3. 检查所有引脚连接

**问题**: 无法唤醒
**解决**:
1. 靠近麦克风清晰说话
2. 使用标准普通话
3. 检查串口日志音频采样情况

**问题**: WiFi连接失败
**解决**:
1. 确认WiFi为2.4GHz
2. 检查密码是否正确
3. 长按BOOT 5秒重置配置

## 进阶配置

### 修改唤醒词
在 `idf.py menuconfig` 中：
```
Xiaozhi Assistant -> Wake Word -> [选择其他唤醒词]
```

### 调整音频参数
编辑 `main/boards/xiaodou/config.h`：
```c
#define AUDIO_INPUT_SAMPLE_RATE  16000  // 麦克风采样率
#define AUDIO_OUTPUT_SAMPLE_RATE 24000  // 扬声器采样率
```

### LED引脚修改
```c
#define BUILTIN_LED_GPIO GPIO_NUM_8  // 改为你的LED引脚
```

## 下一步

- 查看[完整文档](README.md)了解更多功能
- 访问[小智官网](https://xiaozhi.me)注册账号
- 加入社区交流使用心得

## 获取帮助

- GitHub Issues: https://github.com/dakeqi/xiaodou/issues
- 小智官方文档: https://github.com/78/xiaozhi-esp32
- ESP-IDF文档: https://docs.espressif.com/projects/esp-idf/

---

祝您使用愉快！🎉
