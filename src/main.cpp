#include <Arduino.h>
#include <TFT_eSPI.h>
#include <SPI.h>
#include <EEPROM.h>
#include <TimeLib.h>

#include "config.h"
#include "font/timeClockFont.h"

#define Version "SDD V1.0.0"

TFT_eSPI tft = TFT_eSPI();
TFT_eSprite clk = TFT_eSprite(&tft);
#define LCD_BL_PIN 5
uint16_t bgColor = 0x0000;

int LCD_Rotation = 0;
int LCD_BL_PWM = 50;
int prevTime = 0;

int BL_addr = 1;
int Ro_addr = 2;

int counter = 0;
unsigned long lastCounterUpdate = 0;

const int8_t Hour_sign = 60;
const int8_t Minute_sign = 60;

bool tft_output(int16_t x, int16_t y, uint16_t w, uint16_t h, uint16_t *bitmap)
{
  if (y >= tft.height())
    return 0;
  tft.pushImage(x, y, w, h, bitmap);
  return 1;
}

void drawLineFont(uint32_t _x, uint32_t _y, uint32_t _num, uint32_t _size, uint32_t _color)
{
  uint32_t fontSize;
  const LineAtom *fontOne;
  if (_size == 1)
  {
    fontOne = smallLineFont[_num];
    fontSize = smallLineFont_size[_num];
    tft.fillRect(_x, _y, 9, 14, TFT_BLACK);
  }
  else if (_size == 2)
  {
    fontOne = middleLineFont[_num];
    fontSize = middleLineFont_size[_num];
    tft.fillRect(_x, _y, 18, 30, TFT_BLACK);
  }
  else if (_size == 3)
  {
    fontOne = largeLineFont[_num];
    fontSize = largeLineFont_size[_num];
    tft.fillRect(_x, _y, 36, 90, TFT_BLACK);
  }
  else
    return;

  for (uint32_t i = 0; i < fontSize; i++)
  {
    tft.drawFastHLine(fontOne[i].xValue + _x, fontOne[i].yValue + _y, fontOne[i].lValue, _color);
  }
}

void digitalClockDisplay()
{
  int centerX = 48;
  int centerY = 75;

  int d1 = counter / 1000;
  int d2 = (counter / 100) % 10;
  int d3 = (counter / 10) % 10;
  int d4 = counter % 10;

  drawLineFont(centerX, centerY, d1, 3, SD_FONT_WHITE);
  drawLineFont(centerX + 40, centerY, d2, 3, SD_FONT_WHITE);
  drawLineFont(centerX + 82, centerY, d3, 3, SD_FONT_YELLOW);
  drawLineFont(centerX + 122, centerY, d4, 3, SD_FONT_YELLOW);
}

void setup()
{
  Serial.begin(115200);
  EEPROM.begin(512);

  if (EEPROM.read(BL_addr) > 0 && EEPROM.read(BL_addr) < 100)
    LCD_BL_PWM = EEPROM.read(BL_addr);
  if (EEPROM.read(Ro_addr) >= 0 && EEPROM.read(Ro_addr) <= 3)
    LCD_Rotation = EEPROM.read(Ro_addr);

  pinMode(LCD_BL_PIN, OUTPUT);
  analogWrite(LCD_BL_PIN, 1023 - (LCD_BL_PWM * 10));

  tft.init();
  tft.setRotation(LCD_Rotation);
  tft.fillScreen(TFT_BLACK);

  Serial.println("Counter Display v1.0.0");
  Serial.print("Initial counter: ");
  Serial.println(counter);

  digitalClockDisplay();
}

void loop()
{
  if (millis() - lastCounterUpdate >= 1000)
  {
    lastCounterUpdate = millis();
    counter++;
    if (counter > 9999)
      counter = 0;
    digitalClockDisplay();
  }
}
