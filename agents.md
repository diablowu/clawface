# SmallDesktopDisplay 小型桌面显示器

## 项目简介

ESP8266 (NodeMCU V2) 驱动的多功能小桌面显示器，支持天气显示、时间、温湿度、动画等。

## 硬件配置

### 引脚分配
| 引脚 | GPIO |
|------|------|
| SCK  | 14   |
| MOSI | 13   |
| RES  | 2    |
| DC   | 0    |
| LCDBL| 5    |
| DHT11| 12   |
| 按钮 | 4    |

### 开发板
- **Platform**: ESP8266 (ESP-12E)
- **Framework**: Arduino
- **Flash**: 4MB

## 功能特性

- **WiFi配网**: 支持WEB配网 (SmartConfig备用)
- **时间显示**: NTP自动同步 (ntp6.aliyun.com)
- **天气数据**: 从中国天气网获取
- **温湿度**: 支持DHT11传感器 (GPIO12)
- **显示屏**: TFT_eSPI驱动
- **动画**: 右下角动画显示 (太空人/胡桃)
- **存储**: EEPROM保存配置

## 配置选项 (config.h)

| 宏定义 | 默认值 | 说明 |
|--------|--------|------|
| Animate_Choice | 2 | 动图选择: 1=太空人, 2=胡桃 |
| TMS | 1000 | 毫秒基数 |
| WM_EN | 1 | WEB配网使能 |
| DHT_EN | 0 | DHT11传感器使能 |

## Build & Upload

```bash
# 编译
pio run

# 烧录
pio run --target upload --upload-port /dev/ttyCH341USB0

# 串口监视器
pio device monitor --port /dev/ttyCH341USB0 --baud 115200
```

## 串口指令

| 指令 | 功能 |
|------|------|
| 0x01 | 设置亮度 (0-100) |
| 0x02 | 设置城市代码 (9位) |
| 0x03 | 设置屏幕方向 (0-3) |
| 0x04 | 设置天气更新时间 (分钟) |
| 0x05 | 重置WiFi |

## 源代码目录结构

| 目录/文件 | 说明 |
|-----------|------|
| `src/SmallDesktopDisplay.cpp` | 主程序入口 |
| `src/config.h` | 配置文件 |
| `src/Animate/` | 动画模块 - 右下角动态图显示 |
| `src/Animate/img/` | 动画图片资源 (太空人/胡桃) |
| `src/weatherNum/` | 天气图标模块 - 天气状态图标显示 |
| `src/weatherNum/img/tianqi/` | 天气图标资源 (t0-t53) |
| `src/wifiReFlash/` | WiFi配网模块 - WiFi重配网功能 |
| `src/font/` | 字库资源 |
| `src/img/` | 图片资源 (温度/湿度图标) |

## 屏幕方向

- 0: USB接口朝下
- 1: USB接口朝右
- 2: USB接口朝上
- 3: USB接口朝左
