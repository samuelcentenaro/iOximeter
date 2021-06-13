/*******************************************************************************
 * Project: Oxymeter IoT for COVID-19 Monitoring
 * Features: SpO2 and HearBate sensor, Wifi Connections, Oled display, Tago.io
 * uC: ESP32-WROOM-32D
 * IDE: PlatformIO + VSCode 
 * Framework: Arduino
 * Author: Samuel Centenaro de Oliveira
 ******************************************************************************/

/*                   Libraries: basics                                        */
#include <Arduino.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/*                   Libraries: freeRTOS                                      */
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "freertos/semphr.h"

/*                   Libraries: features                                      */
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "MAX30105.h"
#include "spo2_algorithm.h"
#include "heartRate.h"

/*                   Include files for configuration                          */
#include "config\config.h"
#include "config\enums.h"

/*                   Include files for tasks                                  */
#include "tasks\taskWifi.h"
#include "tasks\taskDisplay.h"
#include "tasks\taskSensor.h"

/*                   Include files for functions                              */
#include "functions\oledFunctions.h"
#include "functions\sensorFunctions.h"
#include "functions\serialPrints.h"
#include "functions\setupHardware.h"
#include "functions\wifiFunctions.h"

/*                          FreeRTOS definitions                              */
xTaskHandle task01;      //update display OLED
xTaskHandle task02;      //sensor MAX30105
xTaskHandle task03;      // wifi Connections
SemaphoreHandle_t mutex; // estrutura do semaforo MUTEX
QueueHandle_t fila;      //estrutura da fila do RTOS

/*                          Task prototipes                                   */
void TaskDisplay(void *pvParameters);
void TaskWifi(void *pvParameters);
void measureSensorData(void *pvParameters);

/*        Initialize the OLED display using Arduino Wire:                     */
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

/*                Variable to change the display state                        */
DisplayValues gDisplayValues;
WifiValues gWifiValues;
WiFiClient client;
data_MaxSensor gdata_MaxSensor;

/*                  Sensor MAX3015 definitons and initilizing                 */
MAX30105 particleSensor;

void setup()
{
    /*            This function do all hardware configs                           */
    prvSetupHardware();

    /*                  TASKs Creations                                           */
    xTaskCreatePinnedToCore(
        TaskDisplay,
        "Task_display",
        4096,
        NULL,
        10,
        &task01, 1);

    xTaskCreatePinnedToCore(
        measureSensorData,
        "Task_Sensor",
        10000,
        NULL,
        20,
        &task02, 1);

    xTaskCreatePinnedToCore(
        TaskWifi,
        "Task_Wifi",
        65536,
        NULL,
        15,
        &task03, PRO_CPU_NUM);
}

void loop()
{
    vTaskDelay(10 / portTICK_PERIOD_MS);
}