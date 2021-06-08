/*
Display OLED Functions
https://github.com/ThingPulse/esp8266-oled-ssd1306
https://github.com/olikraus/u8glib/wiki/userreference
*/

#ifndef OLED_FUNCTIONS
#define OLED_FUNCTIONS

#include <Arduino.h>

#include "..\tasks\taskDisplay.h"

/*                               Helpers Functions                            */
#include "..\functions\wifiFunctions.h"

/*                        Libraries OLED                                      */
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
/*                   Include files for configuration                          */
#include "../config/enums.h"
#include "../config/config.h"

//extern SSD1306Wire display;
extern Adafruit_SSD1306 display;

extern WifiValues gWifiValues;

void drawBootScreen(void);
void mainScreen(void);
void draw_Wifi_Signal(void);
void draw_spo2_hb(void);

void drawBootScreen()
{
  display.setTextSize(1); // Draw 2X-scale text
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(10, 5);
  display.println(F("Inicializando"));
  if (gWifiValues.currentState == TRY_CONNECT)
  {
    display.setTextSize(1); // Draw 2X-scale text
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(10, 20);
    display.println(F("Tentando Conectar.."));
  }
  else if (gWifiValues.currentState == CONNECTED)
  {
    display.setTextSize(1); // Draw 2X-scale text
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(10, 20);
    display.println(F("Conectado"));
  }
  else
  {
  }
}

void mainScreen()
{
  //draw_Wifi_Signal();
  draw_spo2_hb();
}

/*           Function: Read Wifi signal and print in the OLED                 */
void draw_Wifi_Signal()
{
  float bars = verifica_sinal_wifi();

  display.drawRect(64, 28, 3, 5, WHITE);
  display.drawRect(69, 23, 3, 10, WHITE);
  display.drawRect(74, 18, 3, 15, WHITE);
  display.drawRect(79, 13, 3, 20, WHITE);
  display.drawRect(84, 8, 3, 25, WHITE);

  for (int b = 0; b <= bars; b++)
  {
    display.fillRect(59 + (b * 5), 33 - (b * 5), 3, b * 5, WHITE);
  }
}

/*              Function to show SPo2 and HB in OLED                          */
void draw_spo2_hb()
{

  display.setTextSize(1); // Draw 2X-scale text
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(5, 17);
  display.println(F("SPo2"));
  display.drawRoundRect(7, 31, 40, 30, 2, WHITE);

  display.setTextSize(1); // Draw 2X-scale text
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(50, 17);
  display.println(F("HB"));

  display.drawRoundRect(52, 31, 40, 30, 2, WHITE);
}

#endif