#ifndef TASK_WIFI_CONNECTION
#define TASK_WIFI_CONNECTION

/*                   Libraries: basics                                        */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <Arduino.h>

/*                   Libraries: freeRTOS                                      */
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

/*                   Libraries: features                                      */
#include "WiFi.h"

/*                   Include files for configuration                          */
#include "../config/enums.h"
#include "../config/config.h"

/*                   Include files for functions                              */
#include "..\functions\wifiFunctions.h"
#include "..\functions\tagoFunction.h"

extern SemaphoreHandle_t mutex;

/*                    Function Prototipe                                      */
void TaskWifi(void *pvParameters);

/*                     Task: Wifi                                             */
void TaskWifi(void *pvParameters)
{
  //xSemaphoreTake(mutex, portMAX_DELAY);
  init_wifi();
  //xSemaphoreGive(mutex);

  while (1)
  {
    vTaskDelay(TEMPO_PARA_VERIFICAR_WIFI / portTICK_PERIOD_MS);
    xSemaphoreTake(mutex, portMAX_DELAY);
    Serial.println("TASK WIFI");
    verifica_conexao_wifi();
    Serial.println(verifica_sinal_wifi());
    send_to_tago();
    xSemaphoreGive(mutex);
    vTaskDelay(100 / portTICK_PERIOD_MS);
  }
}

#endif