#include "wifi_board.h"
#include "codecs/no_audio_codec.h"
#include "application.h"
#include "button.h"
#include "led/single_led.h"
#include "config.h"

#include <wifi_station.h>
#include <esp_log.h>
#include <esp_timer.h>

#define TAG "XiaodouBoard"

// 激活信息输出辅助函数
static void PrintActivationInfo(const char* url, const char* device_id, const char* code) {
    ESP_LOGI(TAG, "");
    ESP_LOGI(TAG, "========================================");
    ESP_LOGI(TAG, "      小豆设备激活信息 (无LCD版本)      ");
    ESP_LOGI(TAG, "========================================");
    ESP_LOGI(TAG, "");
    ESP_LOGI(TAG, "设备ID: %s", device_id ? device_id : "获取中...");
    ESP_LOGI(TAG, "激活码: %s", code ? code : "获取中...");
    ESP_LOGI(TAG, "");
    if (url) {
        ESP_LOGI(TAG, "配网URL: %s", url);
        ESP_LOGI(TAG, "");
        ESP_LOGI(TAG, "请使用以下方式之一完成激活：");
        ESP_LOGI(TAG, "  1. 浏览器访问上述URL");
        ESP_LOGI(TAG, "  2. 使用小智APP扫描二维码");
        ESP_LOGI(TAG, "  3. 在小智APP中手动输入激活码");
        ESP_LOGI(TAG, "");
        ESP_LOGI(TAG, "提示：您可以在串口监控中复制URL");
    }
    ESP_LOGI(TAG, "");
    ESP_LOGI(TAG, "按键说明：");
    ESP_LOGI(TAG, "  - 短按BOOT键：手动唤醒/停止对话");
    ESP_LOGI(TAG, "  - 长按BOOT键5秒：重置WiFi配置");
    ESP_LOGI(TAG, "");
    ESP_LOGI(TAG, "========================================");
    ESP_LOGI(TAG, "");
}

/**
 * 小豆 ESP32-C3 SuperMini 开发板配置（无LCD版本）
 * 
 * 硬件配置:
 * - MAX98357A I2S功放
 * - INMP441 I2S麦克风
 * - 4MB Flash
 * - 单按键交互
 * - 无显示屏（节省成本和引脚）
 */
class XiaodouBoard : public WifiBoard {
private:
    Button boot_button_;
    bool activation_info_printed_ = false;

    void InitializeButtons() {
        boot_button_.OnClick([this]() {
            auto& app = Application::GetInstance();
            if (app.GetDeviceState() == kDeviceStateStarting && !WifiStation::GetInstance().IsConnected()) {
                ResetWifiConfiguration();
            }
            app.ToggleChatState();
        });
    }

    // 检查并输出激活信息（当设备未激活时）
    void CheckAndPrintActivationInfo() {
        // 此函数应在设备启动后被调用，检查是否需要激活
        // 如果需要，则通过串口输出激活信息
        // 注意：实际实现需要从小智系统获取激活信息
        
        if (!activation_info_printed_) {
            // TODO: 从系统获取实际的激活信息
            // const char* url = GetActivationUrl();
            // const char* device_id = GetDeviceId();
            // const char* code = GetActivationCode();
            
            // 示例输出
            ESP_LOGI(TAG, "\n\n\n");
            ESP_LOGI(TAG, "⚠️  无LCD版本激活指南 ⚠️");
            ESP_LOGI(TAG, "");
            ESP_LOGI(TAG, "当设备需要激活时，激活信息将显示在上方的日志中。");
            ESP_LOGI(TAG, "请查找包含 '激活信息' 关键词的日志行。");
            ESP_LOGI(TAG, "");
            ESP_LOGI(TAG, "激活方式：");
            ESP_LOGI(TAG, "  1. 串口方式：通过串口日志查看激活码和URL");
            ESP_LOGI(TAG, "  2. WiFi配网方式：连接设备热点 'XiaoZhi-XXXXXX'");
            ESP_LOGI(TAG, "  3. 语音播报：按BOOT键听取激活码（如已实现）");
            ESP_LOGI(TAG, "");
            ESP_LOGI(TAG, "\n\n\n");
            
            activation_info_printed_ = true;
        }
    }

public:
    XiaodouBoard() : boot_button_(BOOT_BUTTON_GPIO) {
        InitializeButtons();
        ESP_LOGI(TAG, "Xiaodou Board initialized (No LCD version)");
    }

    virtual Led* GetLed() override {
        static SingleLed led(BUILTIN_LED_GPIO);
        return &led;
    }

    virtual Display* GetDisplay() override {
        // 无LCD版本，返回 nullptr
        return nullptr;
    }

    virtual AudioCodec* GetAudioCodec() override {
        static NoAudioCodecSimplex audio_codec(
            AUDIO_INPUT_SAMPLE_RATE, AUDIO_OUTPUT_SAMPLE_RATE,
            AUDIO_I2S_SPK_GPIO_BCLK, AUDIO_I2S_SPK_GPIO_LRCK, AUDIO_I2S_SPK_GPIO_DOUT,
            AUDIO_I2S_MIC_GPIO_SCK, AUDIO_I2S_MIC_GPIO_WS, AUDIO_I2S_MIC_GPIO_DIN);
        return &audio_codec;
    }
};

DECLARE_BOARD(XiaodouBoard);
