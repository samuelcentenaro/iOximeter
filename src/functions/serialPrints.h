#ifndef SERIAL_PRINT
#define SERIAL_PRINT

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include <Arduino.h>
#include "../config/enums.h"
#include "../config/config.h"

/* FreeRTOS -----------------------------------------------*/
//QueueHandle_t xQueue_SPO2; //fila criada para dados de xQueue_SPO2
//SemaphoreHandle_t xSerial_semaphore;
portMUX_TYPE myMutex = portMUX_INITIALIZER_UNLOCKED;

/*
* FUNCTION PROTOTYPES
*/
void vPrintStr(const char *pcString);
void vPrintStrNum(const char *pcString, uint32_t uiValue);
void vPrintStrFloat(const char *pcString, float fValue);
void vPrintHex(uint32_t uiValue);

void vPrintStr(const char *pcString)
{
  portENTER_CRITICAL(&myMutex);
  {
    Serial.print((char *)pcString);
    Serial.flush();
  }
  portEXIT_CRITICAL(&myMutex);
}

/**
  * @brief  : Print strings and int numbers
  * @param  : const char* and uint32_t
  * @retval : void
  */
void vPrintStrNum(const char *pcString, uint32_t uiValue)
{
  portENTER_CRITICAL(&myMutex);
  {
    char buffer[64] = {0};
    sprintf(buffer, "%s %lu\r", pcString, uiValue);
    Serial.println((char *)buffer);
  }
  portEXIT_CRITICAL(&myMutex);
}

/**
  * @brief  : Print strings and float numbers
  * @param  : const char* and float
  * @retval : void
  */
void vPrintStrFloat(const char *pcString, float fValue)
{
  portENTER_CRITICAL(&myMutex);
  {
    char buffer[64] = {0};
    sprintf(buffer, "%s %4.2f\r", pcString, fValue);
    Serial.println((char *)buffer);
  }
  portEXIT_CRITICAL(&myMutex);
}

/**
  * @brief  : Print hexadecimal values
  * @param  : uint32_t
  * @retval : void
  */
void vPrintHex(uint32_t uiValue)
{
  portENTER_CRITICAL(&myMutex);
  {
    char buffer[64] = {0};
    sprintf(buffer, " %08X", uiValue);
    Serial.print((char *)buffer);
  }
  portEXIT_CRITICAL(&myMutex);
}

#endif