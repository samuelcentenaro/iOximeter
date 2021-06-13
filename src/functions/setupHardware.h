#ifndef SETUP_HARDWARE
#define SETUP_HARDWARE

#include <Arduino.h>
#include "../config/enums.h"
#include "../config/config.h"
#include "../functions/serialPrints.h"

/*                   Libraries: freeRTOS                                      */
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "freertos/semphr.h"

#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#include "MAX30105.h"
#include "spo2_algorithm.h"

extern Adafruit_SSD1306 display;

extern MAX30105 particleSensor;

extern DisplayValues gDisplayValues;

extern SemaphoreHandle_t mutex;
extern QueueHandle_t fila;

SysStatus_t stSysStatus =
    {
        .ucChipRev = 0,
        .uiCpuFreq = 0,
        .uiFreeHeapSize = 0,
        .ulMac = 0};

/*Function prototyp*/

void prvSetupHardware(void);

/**
  * @brief  : Setting up UART, GPIOs, Wi-Fi connection, System Infos, etc.
  * @param  : void 
  * @retval : void
  */

void prvSetupHardware(void)
{
  /*                 Inicializa e habilita comunicação serial                 */
#if DEBUG == true
  Serial.begin(115200);
  delay(1000);
#endif

  /*                          Cria semaphor mutex                               */
  mutex = xSemaphoreCreateMutex();
  if (mutex == NULL)
  {
    Serial.println("Falha ao criar semaforo Mutex. ESP será reinicializado..");
    delay(2000);
    ESP.restart();
  }
  else
  {
    Serial.println("Mutex criado com sucesso");
  }

  /*                     Criar lista de filas                                 */
  fila = xQueueCreate(10000, sizeof(double));
  if (fila == NULL)
  {
    Serial.println("Falha ao criar Fila. ESP será reinicializado..");
    delay(2000);
    ESP.restart();
  }
  else
  {
    Serial.println("Fila criada com sucesso");
  }

  /*                       GPIO config                                        */
  pinMode(readLED, OUTPUT);
  pinMode(pulseLED, OUTPUT);
  digitalWrite(readLED, LOW);
  digitalWrite(pulseLED, LOW);

  /*                    Getting ESP32 system informations                     */
  stSysStatus.ucChipRev = ESP.getChipRevision();
  stSysStatus.uiCpuFreq = ESP.getCpuFreqMHz();
  stSysStatus.uiFreeHeapSize = ESP.getFreeHeap();
  stSysStatus.ulMac = ESP.getEfuseMac();

  /*                     BootScreen for ESP32                                 */
  vPrintStr("\n\nXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX\n");
  vPrintStr("******* iOXYMETER - IoT OXYMETER *******************\n");
  vPrintStr("************* Samuel Centenaro de Oliveira**********\n");
  vPrintStr("************ ESP32 + FreeRTOS + MAX30105*************\n");
  vPrintStr("****************************************************\n");
  vPrintStr("----------------------------------------------------\n");
  vPrintStr("****************************************************\n");
  vPrintStr("************* ESP32 SYSTEM INFORMATION *************\n");
  vPrintStr("****************************************************\n");
  vPrintStrNum("Chip revision -----------:", stSysStatus.ucChipRev);
  vPrintStrNum("CPU frequency ------(MHz):", stSysStatus.uiCpuFreq);
  vPrintStrNum("Free Heap Size -------(B):", stSysStatus.uiFreeHeapSize);
  vPrintStr("MAC address -------------:");
  vPrintHex((uint16_t)(stSysStatus.ulMac >> 32));
  vPrintHex(stSysStatus.ulMac);
  vPrintStr("\nWi-Fi -------------------: OK");
  vPrintStr("\nIP ----------------------: ");
  //Serial.println(WiFi.localIP());
  vPrintStr("****************************************************\n");
  vPrintStr("XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX\n\n");

  /*                    OLED Display Initializi                               */
  if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS))
  {
    Serial.println("OLED: initialization failed...");
    Serial.println("ESP will be restart...");
    ESP.restart();
  }
  else
  {
    Serial.println("OLED: successfully initialized");
    display.display();
    delay(3000);                              // Pause for 3 seconds
    display.clearDisplay();                   // Clear the buffer
    display.drawPixel(10, 10, SSD1306_WHITE); // Draw a single pixel in white
    display.display();
    delay(2000);
    gDisplayValues.currentState0 = BOOT_SYSTEM;
    delay(1000);
  }

  /*                 Inicialização do sensor MAX30102                         */
  if (!particleSensor.begin(Wire, I2C_SPEED_FAST)) //Use default I2C port, 400kHz speed
  {
    Serial.println(F("MAX30105 was not found. Please check wiring/power."));
    Serial.println(F("ESP whill be restart..."));
    ESP.restart();
  }
  else
  {
    byte ledBrightness = 0x7F; //Options: 0=Off to 255=50mA
    byte sampleAverage = 4;    //Options: 1, 2, 4, 8, 16, 32
    byte ledMode = 2;          //Options: 1 = Red only, 2 = Red + IR, 3 = Red + IR + Green
    int sampleRate = 200;      //Options: 50, 100, 200, 400, 800, 1000, 1600, 3200
    int pulseWidth = 411;      //Options: 69, 118, 215, 411
    int adcRange = 16384;      //Options: 2048, 4096, 8192, 16384
    // Set up the wanted parameters
    particleSensor.setup(ledBrightness, sampleAverage, ledMode, sampleRate, pulseWidth, adcRange); //Configure sensor with these settings
    particleSensor.enableDIETEMPRDY();

    Serial.println("Sensor MAX inicialidao com sucesso!");
    delay(2000);
  }
}

#endif