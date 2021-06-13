#ifndef CONFIG
#define CONFIG

/**
 * Set this to false to disable Serial logging
 */
#define DEBUG true

/*
* Set this to false do disable Oled Display
*/
#define OLED_DISPLAY true
#define SDA 21
#define SCL 22

/*                            WiFi Credencials                                */
//#define WIFI_NETWORK "CLEANNET-AP 202"
//#define WIFI_PASSWORD "99514988"

//#define WIFI_NETWORK "Pontocomnet - TFS"
//#define WIFI_PASSWORD "24983839"

#define WIFI_NETWORK "AndroidAPB549"
#define WIFI_PASSWORD "tnbm9422"

/*                        OLED Display Definitions                            */
#define SCREEN_WIDTH 128    // OLED display width, in pixels
#define SCREEN_HEIGHT 64    // OLED display height, in pixels
#define OLED_RESET -1       // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C ///< See datasheet for Address;

#define TAMANHO_FILA_POSICOES_SPO2 (28800 / TEMPO_ENTRE_POSICOES_SPO2)

/* definições de temporização das tarefas */
#define TEMPO_PARA_VERIFICAR_WIFI 30000 //ms
#define TICKS_ESPERA_POSICAO_SPO2 (TickType_t)1000
#define TICKS_ESPERA_ENVIO_POSICAO_SPO2 (TickType_t)10000
#define TEMPO_ENTRE_POSICOES_SPO2 3 // time in seconds

/**
 * How long should we wait after a failed WiFi connection
 * before trying to set one up again.
 */
#define WIFI_RECOVER_TIME_MS 20000 // 20 seconds

#if CONFIG_FREERTOS_UNICORE
#define ARDUINO_RUNNING_CORE 0
#else
#define ARDUINO_RUNNING_CORE 1
#endif

#ifndef readLED //Blinks with each data read
#define readLED 13
#endif

#ifndef pulseLED //Blinks with each data read
#define pulseLED 14
#endif

/*                            MAX30105 Definitios                             */
#define MAX_BRIGHTNESS 255
#define TIMETOBOOT 3000 // wait for this time(msec) to output SpO2
#define SCALE 88.0      //adjust to display heart beat and SpO2 in the same scale
#define SAMPLING 5      //if you want to see heart beat more precisely , set SAMPLING to 1
#define FINGER_ON 30000 // if red signal is lower than this , it indicates your finger is not on the sensor
#define MINIMUM_SPO2 80.0

#endif