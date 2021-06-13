#ifndef WIFI_FUNCTIONS
#define WIFI_FUNCTIONS

#include "WiFi.h"
#include <Arduino.h>
#include "../config/enums.h"
#include "../config/config.h"
#include "../functions/serialPrints.h"

extern WifiValues gWifiValues;
extern DisplayValues gDisplayValues;

/* Protótipos de funções gerais */
void init_wifi(void);
void conecta_wifi(void);
void verifica_conexao_wifi(void);
float verifica_sinal_wifi();

void init_wifi(void) //this function do WiFi connection
{
  vPrintStr("------WI-FI -----");
  vPrintStr("Connecting in network:: ");
  vPrintStr(WIFI_NETWORK);
  vPrintStr("Waiting...");
  gWifiValues.currentState = TRY_CONNECT;
  conecta_wifi();
}

void conecta_wifi()
{
  /* Se ja estiver conectado, nada é feito. */
  if (WiFi.status() == WL_CONNECTED)
  {
    gWifiValues.currentState = CONNECTED;
    gDisplayValues.currentState0 = IN_WORKING;
    return;
  }

  /* refaz a conexão */
  WiFi.begin(WIFI_NETWORK, WIFI_PASSWORD);

  while (WiFi.status() != WL_CONNECTED)
  {
    vTaskDelay(100 / portTICK_PERIOD_MS);
    vPrintStr(".");
  }

  vPrintStr("Conectado com sucesso a rede wi-fi ");
  vPrintStr(WIFI_NETWORK);
  vPrintStr("IP: ");
  Serial.println(WiFi.localIP());
}

void verifica_conexao_wifi(void)
{
  conecta_wifi();
}

float verifica_sinal_wifi()
{
  float rssi_value;
  int bars;
  rssi_value = WiFi.RSSI();
  if (rssi_value > -55)
  {
    bars = 4;
  }
  else if (rssi_value<-55 & rssi_value> - 68)
  {
    bars = 3;
  }
  else if (rssi_value<-68 & rssi_value> - 78)
  {
    bars = 2;
  }
  else if (rssi_value<-78 & rssi_value> - 82)
  {
    bars = 1;
  }
  else
  {
    bars = 0;
  }
  return bars;
}

#endif