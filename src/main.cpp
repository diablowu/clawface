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

String displayText = "HELLO WORLD\nESP8266 READY\n> _";

void setup()
{
  Serial.begin(115200);
  delay(500);
  EEPROM.begin(512);

  if (EEPROM.read(BL_addr) > 0 && EEPROM.read(BL_addr) < 100)
    LCD_BL_PWM = EEPROM.read(BL_addr);
  if (EEPROM.read(Ro_addr) >= 0 && EEPROM.read(Ro_addr) <= 3)
    LCD_Rotation = EEPROM.read(Ro_addr);

  pinMode(LCD_BL_PIN, OUTPUT);
  analogWrite(LCD_BL_PIN, 1023 - (LCD_BL_PWM * 10));

  tft.init();
  tft.setRotation(0);
  tft.fillScreen(TFT_BLACK);
  tft.setTextColor(GREEN, TFT_BLACK);

  Serial.println("Terminal Display v1.0.0");
  Serial.println("AT+TEXT=<message> - Set display text");
  Serial.println("AT+CLEAR - Clear screen");
  Serial.println("AT+SHOW - Show current text");

  renderText();
}

void renderText()
{
  int lineCount = 1;
  for (size_t i = 0; i < displayText.length(); i++) {
    if (displayText.charAt(i) == '\n') lineCount++;
  }
  
  while (lineCount > SCREEN_MAX_LINES) {
    int firstNewline = displayText.indexOf('\n');
    if (firstNewline == -1) break;
    displayText = displayText.substring(firstNewline + 1);
    lineCount--;
  }
  
  tft.fillScreen(TFT_BLACK);
  tft.setTextColor(GREEN, TFT_BLACK);
  tft.setTextSize(2);
  
  int16_t cursorX = 0;
  int16_t cursorY = 0;
  const int16_t lineHeight = 20;
  const int16_t charWidth = 12;
  
  for (size_t i = 0; i < displayText.length(); i++)
  {
    char c = displayText.charAt(i);
    if (c == '\n') {
      cursorX = 0;
      cursorY += lineHeight;
    } else {
      if (cursorX + charWidth > tft.width()) {
        cursorX = 0;
        cursorY += lineHeight;
      }
      tft.drawChar(cursorX, cursorY, c, GREEN, TFT_BLACK, 2);
      cursorX += charWidth;
    }
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
