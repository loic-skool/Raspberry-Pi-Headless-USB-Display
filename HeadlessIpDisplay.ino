#include "DEV_Config.h"
#include "GUI_Paint.h"
#include "LCD_1in47.h"

#define DISPLAY_BG_COLOR    BLACK
#define DISPLAY_TEXT_COLOR  WHITE
#define DISPLAY_LABEL_COLOR GRAY

#define LCD_WIDTH  172
#define LCD_HEIGHT 320

uint16_t* framebuffer = nullptr;

String hostname = "Waiting...";
String wifiIp = "";
String ethIp = "";
String wifiMac = "";
String ethMac = "";
String ramUsage = "";

String serialBuffer = "";
unsigned long lastUpdate = 0;
bool dataReceived = false;

String stripCidr(String ip) {
  int slashPos = ip.indexOf('/');
  if (slashPos > 0) return ip.substring(0, slashPos);
  return ip;
}

void setup() {
  DEV_Module_Init();
  LCD_1IN47_Init(HORIZONTAL);
  LCD_1IN47_Clear(DISPLAY_BG_COLOR);

  framebuffer = (uint16_t*)malloc(LCD_WIDTH * LCD_HEIGHT * 2);
  if (framebuffer) {
    Paint_NewImage((uint8_t*)framebuffer, LCD_WIDTH, LCD_HEIGHT, 0, DISPLAY_BG_COLOR);
    Paint_SetScale(65);
    Paint_SetRotate(ROTATE_0);
    Paint_Clear(DISPLAY_BG_COLOR);
  }

  drawDisplay();

  Serial.begin(115200);

  unsigned long startWait = millis();
  while (!Serial && (millis() - startWait < 3000)) {
    delay(10);
  }
}

void loop() {
  while (Serial.available()) {
    char c = Serial.read();
    if (c == '\n') {
      processLine(serialBuffer);
      serialBuffer = "";
    } else if (c != '\r') {
      serialBuffer += c;
    }
  }

  drawDisplay();
}

void processLine(String line) {
  line.trim();
  if (line.length() == 0) return;

  int colonPos = line.indexOf(':');
  if (colonPos == -1) return;

  String key = line.substring(0, colonPos);
  String value = line.substring(colonPos + 1);
  value.trim();
  key.toUpperCase();

  if (key == "HOST") hostname = value;
  else if (key == "WIFI") wifiIp = value;
  else if (key == "ETH") ethIp = value;
  else if (key == "WIFI_MAC") wifiMac = value;
  else if (key == "ETH_MAC") ethMac = value;
  else if (key == "RAM") ramUsage = value;

  dataReceived = true;
  lastUpdate = millis();
}

void drawDisplay() {
  if (!framebuffer) return;

  Paint_Clear(DISPLAY_BG_COLOR);

  int y = 8;
  int lineHeight = 38;

  Paint_DrawString_EN(5, y, "HOST:", &Font16, DISPLAY_BG_COLOR, DISPLAY_LABEL_COLOR);
  y += 18;
  Paint_DrawString_EN(5, y, hostname.c_str(), &Font20, DISPLAY_BG_COLOR, DISPLAY_TEXT_COLOR);
  y += lineHeight;

  Paint_DrawString_EN(5, y, "WIFI:", &Font16, DISPLAY_BG_COLOR, DISPLAY_LABEL_COLOR);
  y += 18;
  Paint_DrawString_EN(5, y, stripCidr(wifiIp).c_str(), &Font16, DISPLAY_BG_COLOR, GREEN);
  y += lineHeight;

  Paint_DrawString_EN(5, y, "ETH:", &Font16, DISPLAY_BG_COLOR, DISPLAY_LABEL_COLOR);
  y += 18;
  Paint_DrawString_EN(5, y, stripCidr(ethIp).c_str(), &Font16, DISPLAY_BG_COLOR, CYAN);
  y += lineHeight;

  Paint_DrawString_EN(5, y, "RAM:", &Font16, DISPLAY_BG_COLOR, DISPLAY_LABEL_COLOR);
  y += 18;
  Paint_DrawString_EN(5, y, ramUsage.c_str(), &Font16, DISPLAY_BG_COLOR, MAGENTA);

  LCD_1IN47_Display(framebuffer);
}
