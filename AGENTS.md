# AGENTS.md - SmallDesktopDisplay Development Guide

## Project Overview

SmallDesktopDisplay is an ESP8266 (NodeMCU V2) based multifunctional desktop display that shows time, weather, temperature/humidity, and animations. It's an Arduino/PlatformIO project targeting embedded systems.

## Build Commands

### Basic Build Operations

```bash
# Build firmware (compiles without uploading)
pio run

# Build for specific environment
pio run -e esp12e

# Clean build
pio run --target clean

# Rebuild (clean + build)
pio run --target rebuild
```

### Upload/Flash Commands

```bash
# Upload to device at specific port
pio run --target upload --upload-port /dev/ttyCH341USB0

# Upload with specific upload speed
pio run --target upload --upload-port /dev/ttyCH341USB0 --upload-speed 3000000

# Upload using esptool directly
esptool.py --port /dev/ttyCH341USB0 --baud 3000000 write_flash 0x00000 sdd.bin
```

### Monitor/Debug

```bash
# Serial monitor
pio device monitor --port /dev/ttyCH341USB0 --baud 115200

# Monitor with filters
pio device monitor --port /dev/ttyCH341USB0 --baud 115200 --filter esp32_exception_decoder
```

### Testing

```bash
# Run tests (if test directory exists)
pio test

# Run specific test file
pio test -f test_filename

# Run tests in verbose mode
pio test -v
```

### Other Commands

```bash
# List connected devices
pio device list

# Size analysis
pio run --target size

# Show built-in targets
pio run --target help
```

## Code Style Guidelines

### General Principles

- This is an embedded C++ project for ESP8266 with limited RAM (80KB) and Flash (4MB)
- Code must be memory-efficient; avoid dynamic memory allocation when possible
- Keep functions small and focused
- Use early returns to reduce nesting

### File Organization

- Main logic in `src/SmallDesktopDisplay.cpp`
- Configuration in `src/config.h`
- Modules in subdirectories: `Animate/`, `weatherNum/`, `wifiReFlash/`
- Fonts and images in `font/` and `img/` directories

### Naming Conventions

- **Files**: lowercase with underscores (e.g., `wifi_re_flash.cpp`)
- **Classes**: PascalCase (e.g., `TFT_eSPI`)
- **Functions**: camelCase (e.g., `getCityWeater`)
- **Variables**: camelCase (e.g., `cityCode`, `updateweater_time`)
- **Constants/Macros**: UPPER_SNAKE_CASE (e.g., `LCD_BL_PIN`, `WM_EN`)
- **Structs**: PascalCase (e.g., `config_type`)

### Code Formatting

- Use 2 spaces for indentation (no tabs)
- Opening brace on same line (K&R style)
- Add spaces around operators (e.g., `a + b`, not `a+b`)
- Maximum line length: 120 characters
- Add Chinese comments for user-facing code (original project is Chinese)

### Header Files

- Use include guards: `#ifndef`, `#define`, `#endif`
- Order includes: standard library → framework → local headers
- Use forward declarations when possible to reduce dependencies

### Types and Variables

- Use appropriate types: `uint8_t`, `uint16_t`, `uint32_t` for integers
- Prefer `int` for loop counters and general use
- Use `float` for temperature/humidity readings
- Use `String` carefully - avoid excessive string concatenation
- Initialize variables at declaration when possible

### Error Handling

- Check return values of network operations
- Validate input ranges (e.g., brightness 0-100, rotation 0-3)
- Use serial output (`Serial.println`) for debugging
- Handle WiFi connection failures gracefully

### Configuration (config.h)

| Macro | Default | Description |
|-------|---------|-------------|
| Animate_Choice | 2 | Animation: 1=astronaut, 2=Hutao |
| TMS | 1000 | Time base in milliseconds |
| WM_EN | 1 | WiFi Manager (WEB config) enabled |
| DHT_EN | 0 | DHT11 sensor enabled |

### Hardware Pin Definitions

| Pin | GPIO | Function |
|-----|------|----------|
| SCK | 14 | SPI Clock |
| MOSI | 13 | SPI MOSI |
| RES | 2 | LCD Reset |
| DC | 0 | LCD Data/Command |
| LCDBL | 5 | LCD Backlight (PWM) |
| DHT11 | 12 | DHT11 sensor (optional) |
| Button | 4 | Input button |

### Dependencies (platformio.ini)

- DHT sensor library
- ArduinoJson
- Button2
- FastLED
- TJpg_Decoder
- Thread (co-routine library)
- TFT_eSPI

### Memory Optimization Tips

- Use PROGMEM for read-only data (fonts, images)
- Avoid recursion
- Reuse sprite buffers instead of creating new ones
- Put large constant arrays in Flash
- Monitor RAM usage with `pio run --target size`

### Serial Commands (for debugging/config)

| Command | Function |
|---------|----------|
| 0x01 | Set brightness (0-100) |
| 0x02 | Set city code (9 digits) |
| 0x03 | Set screen rotation (0-3) |
| 0x04 | Set weather update interval |
| 0x05 | Reset WiFi settings |

### Screen Orientation

- 0: USB connector down
- 1: USB connector right
- 2: USB connector up
- 3: USB connector left

### Version Information

- Current version: SDD V1.4.3
- Target: ESP8266 (ESP-12E) @ 80MHz
- Flash: 4MB

## Common Development Tasks

### Modifying Display Colors

Edit `src/config.h`:
```cpp
#define SD_FONT_YELLOW 0x03FF // RGB565 color format
#define SD_FONT_WHITE 0x001F
```

Colors use RGB565 format: 5-bit red, 6-bit green, 5-bit blue.

### Adding New Features

1. Add configuration macro to `src/config.h`
2. Implement logic in `SmallDesktopDisplay.cpp`
3. Test on hardware
4. Update `agents.md` and `README.md`

### Debugging Tips

- Use serial monitor to see debug output
- Check WiFi connection status
- Verify EEPROM saves correctly
- Test with different screen orientations
