#ifndef TASK_SENSOR
#define TASK_SENSOR

#include "..\config\config.h"
#include "..\config\enums.h"

#include "..\functions\sensorFunctions.h"

/*                   Libraries: freeRTOS                                      */
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "freertos/semphr.h"

#include <Wire.h>
#include "MAX30105.h"
#include "spo2_algorithm.h"

extern MAX30105 particleSensor;

extern SemaphoreHandle_t mutex;

void measureSensorData(void *parameter);

void measureSensorData(void *parameter)
{
  while (1)
  {
    xSemaphoreTake(mutex, portMAX_DELAY);
    Serial.println("Mutex sensor");
    sensorFunction();
    // delay(1000);
    xSemaphoreGive(mutex);
    vTaskDelay(10 / portTICK_PERIOD_MS);
  }
}

#endif