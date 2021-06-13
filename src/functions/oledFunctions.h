/*
Display OLED Functions
https://github.com/ThingPulse/esp8266-oled-ssd1306
https://github.com/olikraus/u8glib/wiki/userreference
*/

#ifndef OLED_FUNCTIONS
#define OLED_FUNCTIONS

#include <Arduino.h>

/*                   Libraries: freeRTOS                                      */
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "freertos/semphr.h"

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
#include "../config/images.h"

//extern SSD1306Wire display;
extern Adafruit_SSD1306 display;

extern WifiValues gWifiValues;

extern DisplayValues gDisplayValues;
extern data_MaxSensor gdata_MaxSensor;

extern QueueHandle_t fila;

void drawBootScreen(void);
void mainScreen(void);
void draw_Wifi_Signal(void);
void draw_spo2_hb(void);
void draw_status(void);
void draw_send(void);

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

  draw_spo2_hb();
  draw_Wifi_Signal();
  draw_status();
  draw_send();
}

/*           Function: Read Wifi signal and print in the OLED                 */
void draw_Wifi_Signal()
{
  float bars = verifica_sinal_wifi();

  display.drawRect(109, 12, 4, 4, WHITE);
  display.drawRect(114, 8, 4, 8, WHITE);
  display.drawRect(119, 4, 4, 12, WHITE);
  display.drawRect(124, 0, 4, 16, WHITE);

  for (int b = 0; b <= bars; b++)
  {
    display.fillRect(104 + (b * 5), 16 - (b * 4), 4, b * 4, WHITE);
  }
}

/*              Function to show SPo2 and HB in OLED                          */
void draw_spo2_hb()
{
  double data_sensor = 0;

  if (xQueueReceive(fila, &(gdata_MaxSensor), (TickType_t)10000))
  {
    Serial.println("Fila recebida..");
  }
  else
    Serial.println("Erro ao receber fila");
  if (gDisplayValues.currentState1 == FINGER_IN)
  {
    if (gdata_MaxSensor.spo2 > 94.5 && gdata_MaxSensor.spo2 <= 100)
      gDisplayValues.currentState1 = SPO2_OK;
    else if (gdata_MaxSensor.spo2 <= 94.5)
      gDisplayValues.currentState1 = SPO2_DANGER;
    else
      gDisplayValues.currentState1 = ERROR_2;
  }
  else
    gDisplayValues.currentState1 == NO_FINGER;

  display.setTextSize(1); // Draw 2X-scale text
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(2, 4);
  display.println(F("% SPo2"));

  display.setTextSize(1); // Draw 2X-scale text
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(8, 30);
  display.println(gdata_MaxSensor.spo2, 1);

  display.drawRoundRect(2, 20, 40, 30, 2, WHITE);

  display.setTextSize(1); // Draw 2X-scale text
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(58, 5);
  display.println(F("BPM"));

  display.drawRoundRect(52, 20, 40, 30, 2, WHITE);
}

void draw_status()
{
  //display.drawRoundRect(2, 52, 90, 12, 2, WHITE);
  display.setTextSize(1); // Draw 2X-scale text
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(4, 55);
  display.println(F("STATUS: "));
  switch (gDisplayValues.currentState1)
  {
  case NO_FINGER:
    display.setTextSize(1); // Draw 2X-scale text
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(50, 55);
    display.println(F("No finger!"));
    break;

  case SPO2_OK:
    display.setTextSize(1); // Draw 2X-scale text
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(50, 55);
    display.println(F("SPo2 OK!"));
    break;

  case SPO2_DANGER:
    display.setTextSize(1); // Draw 2X-scale text
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(50, 55);
    display.println(F("SPo2 LOW!"));
    break;

  default:
    display.setTextSize(1); // Draw 2X-scale text
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(50, 55);
    display.println(F("ERROR!"));
    break;
  }
}

void draw_send()
{
  display.drawBitmap(32, 90, send_data_oled, 28, 19, WHITE);
}

#endif
