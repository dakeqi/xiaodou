# 校庆智能花瓶台灯 - 低成本方案

## 方案概述

**核心思路**：
1. **AI对话**：校友自己绑定小智官方（学校零成本）
2. **校庆内容**：部署到腾讯云开发云存储（极低成本）
3. **本地功能**：台灯控制、氛围灯等（无需联网）

---

## 📊 架构设计

```
┌─────────────────────────────────────────────────────────────────┐
│                      智能花瓶台灯系统架构                         │
├─────────────────────────────────────────────────────────────────┤
│                                                                  │
│   ┌──────────────┐        ┌──────────────────────┐              │
│   │  小智官方服务  │        │   腾讯云开发云存储    │              │
│   │  xiaozhi.me  │        │                      │              │
│   │              │        │  /school-assets/     │              │
│   │ • AI对话     │        │  ├── audio/          │              │
│   │ • 语音识别   │        │  │   ├── 校歌.mp3     │              │
│   │ • 语音合成   │        │  │   ├── 校史.mp3     │              │
│   │ • 唤醒词     │        │  │   └── 祝福.mp3     │              │
│   │              │        │  └── text/           │              │
│   │  (校友自绑)   │        │      └── intro.json  │              │
│   └──────┬───────┘        └──────────┬───────────┘              │
│          │                           │                          │
│          │         WiFi              │                          │
│          ▼                           ▼                          │
│   ┌─────────────────────────────────────────────┐               │
│   │         智能花瓶台灯 (ESP32-C3)              │               │
│   │  ┌─────────────────────────────────────┐   │               │
│   │  │           固件功能模块               │   │               │
│   │  ├─────────────────────────────────────┤   │               │
│   │  │ 🎤 语音唤醒      │ 本地ESP-SR        │   │               │
│   │  │ 🤖 AI对话        │ → 小智官方        │   │               │
│   │  │ 🎵 播放校歌      │ → 腾讯云存储      │   │               │
│   │  │ 📖 学校介绍      │ → 腾讯云存储      │   │               │
│   │  │ 💡 台灯控制      │ 本地GPIO          │   │               │
│   │  │ 🌈 氛围灯效      │ 本地WS2812        │   │               │
│   │  │ ⏰ 定时功能      │ 本地RTC           │   │               │
│   │  └─────────────────────────────────────┘   │               │
│   └─────────────────────────────────────────────┘               │
│                                                                  │
└─────────────────────────────────────────────────────────────────┘
```

---

## 💰 成本分析

### 运营成本对比

| 方案 | 月成本 | 年成本 | 500台规模 |
|------|-------|-------|----------|
| ❌ 私有服务器 | ¥300-500 | ¥3600-6000 | 高昂 |
| ✅ 本方案 | **¥5-20** | **¥60-240** | 极低 |

### 腾讯云开发云存储详细费用

| 计费项 | 单价 | 预估用量 | 月费用 |
|-------|------|---------|-------|
| 存储容量 | ¥0.0043/GB/天 | 100MB | ¥0.13 |
| 下载流量 | ¥0.21/GB | 50GB/月 | ¥10.50 |
| CDN流量 | ¥0.18/GB | 可选 | 可选 |
| **月总计** | - | - | **≈¥11** |

> 💡 **免费额度**：腾讯云开发提供 5GB存储 + 5GB/月下载流量，小规模完全免费！

### 硬件成本（单台）

| 组件 | 成本 |
|------|------|
| ESP32-C3 SuperMini | ¥15 |
| MAX98357A + 扬声器 | ¥10 |
| INMP441 麦克风 | ¥8 |
| LED灯/氛围灯 | ¥10 |
| 花瓶外壳 | ¥30 |
| 其他配件 | ¥10 |
| **单台总计** | **¥83** |

---

## 🔧 技术实现

### 1. 腾讯云开发配置

#### 步骤1：创建云开发环境

```bash
# 1. 访问腾讯云开发控制台
https://console.cloud.tencent.com/tcb

# 2. 创建环境（选择按量计费，有免费额度）
环境名称：school-anniversary
地域：选择就近

# 3. 获取环境ID
例如：school-xxxxx
```

#### 步骤2：上传校庆资源

```
云存储目录结构：
/school-assets/
├── audio/
│   ├── school_song.mp3        # 校歌 (约3MB)
│   ├── school_history.mp3     # 校史介绍 (约5MB)
│   ├── welcome.mp3            # 欢迎语 (约1MB)
│   └── blessing.mp3           # 校庆祝福 (约2MB)
├── text/
│   ├── intro.json             # 学校简介文本
│   └── history.json           # 校史文本
└── config/
    └── device_config.json     # 设备配置
```

#### 步骤3：获取访问地址

```
文件访问URL格式：
https://[环境ID].tcb.qcloud.la/school-assets/audio/school_song.mp3

示例：
https://school-xxxxx.tcb.qcloud.la/school-assets/audio/school_song.mp3
```

### 2. 固件配置

#### config.h 添加云存储配置

```cpp
// ============================================================
// 腾讯云开发云存储配置
// ============================================================

// 云存储基础URL
#define CLOUD_STORAGE_BASE_URL "https://school-xxxxx.tcb.qcloud.la/school-assets"

// 音频资源路径
#define AUDIO_SCHOOL_SONG      "/audio/school_song.mp3"
#define AUDIO_SCHOOL_HISTORY   "/audio/school_history.mp3"
#define AUDIO_WELCOME          "/audio/welcome.mp3"
#define AUDIO_BLESSING         "/audio/blessing.mp3"

// 是否启用校庆特色功能
#define ENABLE_SCHOOL_FEATURES  1

// 学校名称（用于语音提示）
#define SCHOOL_NAME "XX大学"
#define ANNIVERSARY_YEAR "70"
```

#### 校庆功能模块实现

```cpp
// school_features.h
#ifndef SCHOOL_FEATURES_H
#define SCHOOL_FEATURES_H

#include <string>
#include "config.h"

class SchoolFeatures {
public:
    // 播放校歌
    static void PlaySchoolSong();
    
    // 播放校史介绍
    static void PlaySchoolHistory();
    
    // 播放欢迎语
    static void PlayWelcome();
    
    // 播放校庆祝福
    static void PlayBlessing();
    
    // 检查是否是校庆相关指令
    static bool IsSchoolCommand(const std::string& text);
    
    // 处理校庆相关指令
    static bool HandleSchoolCommand(const std::string& text);
    
private:
    // 从云存储播放音频
    static void PlayFromCloud(const char* path);
    
    // 构建完整URL
    static std::string BuildUrl(const char* path);
};

#endif
```

```cpp
// school_features.cc
#include "school_features.h"
#include "audio_player.h"
#include "http_client.h"
#include <esp_log.h>
#include <cstring>

#define TAG "SchoolFeatures"

std::string SchoolFeatures::BuildUrl(const char* path) {
    return std::string(CLOUD_STORAGE_BASE_URL) + path;
}

void SchoolFeatures::PlayFromCloud(const char* path) {
    std::string url = BuildUrl(path);
    ESP_LOGI(TAG, "Playing from cloud: %s", url.c_str());
    
    // 使用HTTP流式播放或下载后播放
    AudioPlayer::GetInstance().PlayUrl(url.c_str());
}

void SchoolFeatures::PlaySchoolSong() {
    ESP_LOGI(TAG, "Playing school song");
    PlayFromCloud(AUDIO_SCHOOL_SONG);
}

void SchoolFeatures::PlaySchoolHistory() {
    ESP_LOGI(TAG, "Playing school history");
    PlayFromCloud(AUDIO_SCHOOL_HISTORY);
}

void SchoolFeatures::PlayWelcome() {
    ESP_LOGI(TAG, "Playing welcome message");
    PlayFromCloud(AUDIO_WELCOME);
}

void SchoolFeatures::PlayBlessing() {
    ESP_LOGI(TAG, "Playing anniversary blessing");
    PlayFromCloud(AUDIO_BLESSING);
}

bool SchoolFeatures::IsSchoolCommand(const std::string& text) {
    // 检测校庆相关关键词
    const char* keywords[] = {
        "校歌", "播放校歌", "唱校歌",
        "学校介绍", "介绍学校", "学校简介",
        "校史", "学校历史",
        "校庆", "校庆祝福", "周年庆"
    };
    
    for (const char* keyword : keywords) {
        if (text.find(keyword) != std::string::npos) {
            return true;
        }
    }
    return false;
}

bool SchoolFeatures::HandleSchoolCommand(const std::string& text) {
    if (text.find("校歌") != std::string::npos) {
        PlaySchoolSong();
        return true;
    }
    
    if (text.find("学校介绍") != std::string::npos || 
        text.find("介绍学校") != std::string::npos ||
        text.find("学校简介") != std::string::npos) {
        PlaySchoolHistory();
        return true;
    }
    
    if (text.find("校史") != std::string::npos || 
        text.find("学校历史") != std::string::npos) {
        PlaySchoolHistory();
        return true;
    }
    
    if (text.find("校庆祝福") != std::string::npos || 
        text.find("周年庆") != std::string::npos) {
        PlayBlessing();
        return true;
    }
    
    return false;
}
```

### 3. 集成到主程序

```cpp
// 在处理语音识别结果时，优先检查校庆指令
void OnVoiceRecognized(const std::string& text) {
    ESP_LOGI(TAG, "Recognized: %s", text.c_str());
    
    #if ENABLE_SCHOOL_FEATURES
    // 优先处理校庆相关指令（本地处理，不走AI）
    if (SchoolFeatures::HandleSchoolCommand(text)) {
        ESP_LOGI(TAG, "Handled by SchoolFeatures");
        return;
    }
    #endif
    
    // 其他指令走小智官方AI服务
    SendToXiaozhiServer(text);
}
```

---

## 📱 校友使用流程

### 开箱到使用完整流程

```
┌─────────────────────────────────────────────────────────────┐
│                    校友使用流程                              │
├─────────────────────────────────────────────────────────────┤
│                                                              │
│  📦 第1步：开箱                                              │
│  └─ 取出智能花瓶台灯，插上电源                                │
│                                                              │
│  📶 第2步：配网                                              │
│  ├─ 设备LED快闪，进入配网模式                                 │
│  ├─ 手机WiFi连接 "XiaoZhi-XXXXXX"                           │
│  ├─ 自动弹出配网页面                                         │
│  └─ 输入家庭WiFi名称和密码，提交                              │
│                                                              │
│  🔗 第3步：绑定小智（可选，享受AI功能）                        │
│  ├─ 访问 xiaozhi.me 注册/登录                                │
│  ├─ 点击"添加设备"                                           │
│  ├─ 输入设备上的6位激活码                                     │
│  └─ 绑定成功！                                               │
│                                                              │
│  🎉 第4步：开始使用                                          │
│  ├─ 说"你好小智" → 唤醒设备                                   │
│  ├─ 说"播放校歌" → 播放学校校歌 ♪                             │
│  ├─ 说"介绍学校" → 播放学校介绍                               │
│  ├─ 说"开灯/关灯" → 控制台灯                                  │
│  └─ 说任何问题 → AI智能回答                                   │
│                                                              │
└─────────────────────────────────────────────────────────────┘
```

### 使用说明卡设计

```
┌─────────────────────────────────────────┐
│                                         │
│    🎓 XX大学70周年校庆纪念              │
│       智能花瓶台灯                       │
│                                         │
├─────────────────────────────────────────┤
│                                         │
│  📱 快速配网                            │
│  1. 插电，等待灯光快闪                   │
│  2. 手机连接WiFi: XiaoZhi-XXXXXX       │
│  3. 在弹出页面输入家庭WiFi密码           │
│                                         │
│  🤖 绑定AI助手（可选）                   │
│  访问 xiaozhi.me                        │
│  输入激活码：______                      │
│                                         │
│  🗣️ 语音指令                            │
│  • "你好小智" - 唤醒                     │
│  • "播放校歌" - 听校歌                   │
│  • "介绍学校" - 学校简介                 │
│  • "开灯/关灯" - 台灯控制                │
│  • "调亮/调暗" - 亮度调节                │
│                                         │
│  💡 LED状态                             │
│  • 快闪 = 配网中                         │
│  • 常亮 = 已连接                         │
│  • 呼吸 = 对话中                         │
│                                         │
│  ❓ 问题反馈                             │
│  校友会邮箱: alumni@xxu.edu.cn          │
│                                         │
└─────────────────────────────────────────┘
```

---

## 📁 云存储资源准备清单

### 音频文件制作

| 文件名 | 内容 | 时长 | 格式 | 大小 |
|-------|------|------|------|------|
| school_song.mp3 | 校歌 | 3-4分钟 | MP3 128kbps | ~3MB |
| school_history.mp3 | 校史介绍 | 5-8分钟 | MP3 128kbps | ~6MB |
| welcome.mp3 | 欢迎语 | 30秒 | MP3 128kbps | ~0.5MB |
| blessing.mp3 | 校庆祝福 | 1-2分钟 | MP3 128kbps | ~1.5MB |

### 音频制作建议

```
欢迎语示例：
"欢迎回家，亲爱的校友！我是XX大学70周年校庆纪念版智能助手。
感谢您对母校的关心与支持。您可以说'播放校歌'听听我们的校歌，
或者说'介绍学校'了解母校的发展历程。有任何问题，尽管问我！"

校歌介绍语：
"下面为您播放XX大学校歌——《歌曲名称》"
[播放校歌]
"校歌播放完毕，您还想听什么？"
```

### TTS工具推荐

如需生成语音：
- 阿里云TTS（有免费额度）
- 腾讯云TTS（有免费额度）
- Edge TTS（免费）
- 真人录制（效果最好）

---

## ✅ 实施步骤

### 第一阶段：准备（1周）

- [ ] 创建腾讯云开发账号
- [ ] 创建云开发环境
- [ ] 准备音频资源（校歌、介绍等）
- [ ] 上传资源到云存储
- [ ] 测试资源访问

### 第二阶段：开发（2周）

- [ ] 修改固件配置，添加云存储URL
- [ ] 实现SchoolFeatures模块
- [ ] 集成到主程序
- [ ] 本地测试完整功能
- [ ] 测试云端资源播放

### 第三阶段：生产（2-3周）

- [ ] 采购硬件物料
- [ ] 批量烧录固件
- [ ] 组装测试
- [ ] 包装

### 第四阶段：发放

- [ ] 准备使用说明卡
- [ ] 校庆活动发放

---

## 🎯 方案优势总结

| 优势 | 说明 |
|------|------|
| ✅ 零服务器成本 | 校友自绑小智官方 |
| ✅ 极低存储成本 | 腾讯云开发免费额度基本够用 |
| ✅ 校庆特色 | 校歌、校史等专属内容 |
| ✅ 简单维护 | 只需维护云存储资源 |
| ✅ 持续可用 | 依托小智官方长期服务 |
| ✅ 校友自主 | 校友自己管理设备 |

---

## 📞 技术支持

- 腾讯云开发文档：https://cloud.tencent.com/document/product/876
- 小智官方：xiaozhi.ai@tenclass.com
- 项目仓库：https://github.com/dakeqi/xiaodou

---

**这个方案让学校以极低成本（每月约¥10-20）就能为校友提供具有校庆特色的智能礼物！** 🎓🎉
