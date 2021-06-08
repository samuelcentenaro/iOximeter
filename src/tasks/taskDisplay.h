#ifndef TASK_DISPLAY
#define TASK_DISPLAY

#include "..\functions\oledFunctions.h"

#include "..\config\config.h"
#include "..\config\enums.h"

#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

/*                   Libraries: freeRTOS                                      */
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "freertos/semphr.h"

extern Adafruit_SSD1306 display;
extern DisplayValues gDisplayValues;

extern SemaphoreHandle_t mutex;

void TaskDisplay(void *pvParameters)
{

  for (;;)
  {
    xSemaphoreTake(mutex, portMAX_DELAY);
    Serial.println("OLED update...");

    if (gDisplayValues.currentState == BOOT_SYSTEM)
    {
      display.clearDisplay();
      drawBootScreen();
      display.display();
    }
    else if (gDisplayValues.currentState == IN_WORKING)
    {
      display.clearDisplay();
      mainScreen();
      display.display();
    }
    else
    {
    }

    // Sleep for 1 seconds, then update display again!
    // vTaskDelay(1000 / portTICK_PERIOD_MS);
    xSemaphoreGive(mutex);
    vTaskDelay(10 / portTICK_PERIOD_MS);
  }
}

#endif