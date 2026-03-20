#include <Arduino.h>
#include <TFT_eSPI.h>
#include <SPI.h>
#include <EEPROM.h>

#include "config.h"
#include "font/timeClockFont.h"

#define Version "SDD V1.0.0"

TFT_eSPI tft = TFT_eSPI();
#define LCD_BL_PIN 5
#define GREEN 0x07E0

int LCD_Rotation = 0;
int LCD_BL_PWM = 50;

int BL_addr = 1;
int Ro_addr = 2;

void renderText();

String displayText = "HELLO WORLD\r\nESP8266 READY\r\n> _";

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

  tft.setTextSize(1);
  tft.setTextColor(GREEN, TFT_BLACK);

  Serial.println("Terminal Display v1.0.0");
  Serial.println("AT+TEXT=<message> - Set display text");
  Serial.println("AT+CLEAR - Clear screen");
  Serial.println("AT+SHOW - Show current text");

  renderText();
}

void renderText()
{
  tft.fillScreen(TFT_BLACK);
  tft.setTextColor(GREEN, TFT_BLACK);
  tft.setTextSize(2);
  
  int16_t cursorX = 0;
  int16_t cursorY = 0;
  const int16_t lineHeight = 20;
  const int16_t charWidth = 12;
  const int16_t maxLines = tft.height() / lineHeight;
  
  int lineCount = 0;
  for (size_t i = 0; i < displayText.length(); i++)
  {
    char c = displayText.charAt(i);
    if (c == '\r') {
      cursorX = 0;
    } else if (c == '\n') {
      cursorX = 0;
      cursorY += lineHeight;
      lineCount++;
    } else {
      if (cursorX + charWidth > tft.width()) {
        cursorX = 0;
        cursorY += lineHeight;
        lineCount++;
      }
      tft.drawChar(cursorX, cursorY, c, GREEN, TFT_BLACK, 2);
      cursorX += charWidth;
    }
  }
  
  if (lineCount >= maxLines && cursorY >= tft.height()) {
    displayText = displayText.substring(displayText.indexOf('\n') + 1);
    delay(50);
    renderText();
  }
}

void handleSerial()
{
  if (Serial.available())
  {
    String cmd = Serial.readStringUntil('\n');
    cmd.trim();
    cmd.toUpperCase();
    
    if (cmd.startsWith("AT+TEXT="))
    {
      String text = cmd.substring(8);
      text.replace("\\r", "\r");
      text.replace("\\n", "\n");
      if (displayText.length() > 0 && !displayText.endsWith("\n")) {
        displayText += "\n";
      }
      displayText += text;
      renderText();
      Serial.println("OK");
    }
    else if (cmd == "AT+CLEAR")
    {
      displayText = "";
      renderText();
      Serial.println("OK");
    }
    else if (cmd == "AT+SHOW")
    {
      Serial.println(displayText);
    }
    else if (cmd == "AT")
    {
      Serial.println("OK");
    }
    else if (cmd.startsWith("AT"))
    {
      Serial.println("ERROR: Unknown command");
    }
    else
    {
      displayText += cmd + "\r\n";
      renderText();
    }
  }
}

void loop()
{
  handleSerial();
}
