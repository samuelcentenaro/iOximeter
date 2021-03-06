#ifndef TAGO_FUNCTION
#define TAGO_FUNCTION

#include <DNSServer.h>
#include <WiFi.h>
#include <WiFiClient.h>

/*                   Include files for configuration                          */
#include "config\config.h"
#include "config\enums.h"
#include "config\images.h"

/*                 Definições para conexão e integração no TAGO.io            */
/* String apiKey = "51b1d45d-44fa-4380-9716-e676adb5488d"; */

String apiKey = "2de33d42-8e0c-4225-a83e-34362d60d87d";

const char *server = "api.tago.io";

extern WiFiClient client;
extern QueueHandle_t fila;

extern data_MaxSensor gdata_MaxSensor;

void send_to_tago();

void send_to_tago()
{
  //double data_sensor = 0;

  if (xQueueReceive(fila, &(gdata_MaxSensor), (TickType_t)10000))
  {
    Serial.println("Fila recebida..");
    Serial.println("A:" + String(gdata_MaxSensor.spo2));
    Serial.println("B:" + String(gdata_MaxSensor.heartbeat));
    Serial.println("C:" + String(gdata_MaxSensor.valid_heartbeat));
    Serial.println("D:" + String(gdata_MaxSensor.valid_spo2));
  }
  else
    Serial.println("Erro ao receber fila");

  if (client.connect(server, 80))
  {
    Serial.print("CONNECTED AT TAGO..\n");
    String postStr = "";
    String postData = "";

    /*                              JSON data formatation                     */
    postData = "[\n{\n\"variable\":\"fspo2\",\n\"value\":" + String(gdata_MaxSensor.spo2) + "\n},\n";
    postData += "{\n\"variable\":\"fspo2_v\",\n\"value\":" + String(gdata_MaxSensor.valid_heartbeat) + "\n},\n";
    postData += "{\n\"variable\":\"fhb\",\n\"value\":" + String("3") + "\n},\n";
    postData += "{\n\"variable\":\"fbh_v\",\n\"value\":" + String("4") + "\n}\n]";
    postStr = "POST /data HTTP/1.1\n";
    postStr += "Host: api.tago.io\n";
    postStr += "Device-Token: " + apiKey + "\n";
    postStr += "_ssl: false\n";
    postStr += "Content-Type: application/json\n";
    postStr += "Content-Length: " + String(postData.length()) + "\n";
    postStr += "\n";
    postStr += postData;

    client.print(postStr);
    Serial.println("Dados postados em TAGO");

    /*  unsigned long timeout = millis();

    while (client.available() == 0)
    {
      if (millis() - timeout > 5000)
      {
        Serial.println(">>> Client Timeout !");
        client.stop();
        return;
      }
    }

    while (client.available())
    {
      String line = client.readStringUntil('\r');
      //Serial.print(line);
    } */
  }
  client.stop();
}
#endif