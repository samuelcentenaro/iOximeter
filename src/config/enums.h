#ifndef ENUMS
#define ENUMS

#include <Arduino.h>

enum DEVICE_STATE
{
  BOOT_SYSTEM,
  WIFI_CONNECT,
  IN_WORKING,
  ERROR_SYSTEM
};

enum WIFI_STATUS
{
  NOT_CONNECT,
  TRY_CONNECT,
  CONNECTED,
  ERROR
};

enum DISPLAY_STATUS
{
  NO_FINGER,
  FINGER_IN,
  SPO2_OK,
  SPO2_DANGER,
  ERROR_2
};

/*
Struts
*/

// Place to store all the variables that need to be displayed.
// All other functions should update these!
struct DisplayValues
{
  double spo2;
  double bpm;
  DEVICE_STATE currentState0;
  DISPLAY_STATUS currentState1;
};

struct WifiValues
{
  byte RSSI;
  WIFI_STATUS currentState;
};

typedef struct
{
  uint8_t ucChipRev;      //ESP32 chip revision
  uint8_t uiCpuFreq;      //CPU frequency value
  uint8_t uiFreeHeapSize; //Free heap memory uiFreeHeapSize
  uint8_t ulMac;          //MAC adrees of ESP32
} SysStatus_t;

/*                            Estrutura dos dados MAX10302                    */
typedef struct
{
  double spo2 = 0;
  byte valid_spo2 = 0;
  double heartbeat = 0;
  byte valid_heartbeat = 0;
} data_MaxSensor;

#if DEBUG == true
#define serial_print(x) Serial.print(x)
#define serial_println(x) Serial.println(x)
#else
#define serial_print(x)
#define serial_println(x)
#endif
#endif
