# SmallDesktopDisplay 小型桌面显示器

ESP8266 (NodeMCU V2) 驱动的多功能小桌面显示器。

## 烧录固件

### 使用 esptool.py

```bash
esptool.py --port /dev/ttyCH341USB0 --baud 3000000 write_flash 0x00000 sdd.bin
```

### 使用 PlatformIO

```bash
pio run --target upload --upload-port /dev/ttyCH341USB0
```

## 串口监视器

```bash
pio device monitor --port /dev/ttyCH341USB0 --baud 115200
```
