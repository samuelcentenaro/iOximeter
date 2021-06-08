/*
https://github.com/baetis-ma/esp32-max30102-website/blob/master/main/heartrate.c
https://hackaday.io/project/164155-esp-heart-rate-monitor/details
https://github.com/jeffmer/tinyPulsePPG
https://www.hackster.io/rswm/diy-blood-oximeter-93529b




*/

#ifndef SENSOR_FUNCTIONS
#define SENSOR_FUNCTIONS

#include <Arduino.h>
#include <Wire.h>
#include "MAX30105.h"  //MAX3010x library
#include "heartRate.h" //Heart rate calculating algorithm

#include "config\config.h"
#include "config\enums.h"
#include "config\images.h"

extern MAX30105 particleSensor;

extern SemaphoreHandle_t mutex;

void sensorFunction(void);

double avered = 0;
double aveir = 0;
double sumirrms = 0;
double sumredrms = 0;
int i = 0;
int Num = 100; //calculate SpO2 by this sampling interval

double ESpO2 = 95.0; //initial value of estimated SpO2
double FSpO2 = 0.7;  //filter factor for estimated SpO2
double frate = 0.95; //low pass filter for IR/red LED value to eliminate AC component

const byte RATE_SIZE = 4; //Increase this for more averaging. 4 is good.
byte rates[RATE_SIZE];    //Array of heart rates
byte rateSpot = 0;
long lastBeat = 0; //Time at which the last beat occurred
float beatsPerMinute;
int beatAvg;

void sensorFunction()
{
  boolean sensor = true;
  Serial.println("Função SENSOR: " + String(sensor));

  while (sensor == true)
  {
    uint32_t ir, red, green;
    double fred, fir;
    double SpO2 = 0;

    particleSensor.check(); //Check the sensor, read up to 3 samples

    while (particleSensor.available())
    {
      red = particleSensor.getFIFORed(); //Sparkfun's MAX30105
      ir = particleSensor.getFIFOIR();   //Sparkfun's MAX30105

      i++;
      fred = (double)red;
      fir = (double)ir;
      avered = avered * frate + (double)red * (1.0 - frate); //average red level by low pass filter
      aveir = aveir * frate + (double)ir * (1.0 - frate);    //average IR level by low pass filter
      sumredrms += (fred - avered) * (fred - avered);        //square sum of alternate component of red level
      sumirrms += (fir - aveir) * (fir - aveir);             //square sum of alternate component of IR level
      if ((i % SAMPLING) == 0)
      { //slow down graph plotting speed for arduino Serial plotter by thin out
        if (millis() > TIMETOBOOT)
        {
          float ir_forGraph = (2.0 * fir - aveir) / aveir * SCALE;
          float red_forGraph = (2.0 * fred - avered) / avered * SCALE;
          //trancation for Serial plotter's autoscaling
          if (ir_forGraph > 100.0)
            ir_forGraph = 100.0;
          if (ir_forGraph < 80.0)
            ir_forGraph = 80.0;
          if (red_forGraph > 100.0)
            red_forGraph = 100.0;
          if (red_forGraph < 80.0)
            red_forGraph = 80.0;
          //Serial.print(red);
          // Serial.print(",");
          //Serial.print(ir);
          // Serial.print(".");
          if (ir < FINGER_ON)
            ESpO2 = MINIMUM_SPO2; //indicator for finger detached
          float temperature = particleSensor.readTemperatureF();
          // Serial.print(" Oxygen % = ");
          //Serial.println(ESpO2);
        }
      }
      if ((i % Num) == 0)
      {
        double R = (sqrt(sumredrms) / avered) / (sqrt(sumirrms) / aveir);
        //Serial.println(R);
        // SpO2 = -23.3 * (R - 0.4) + 100; //http://ww1.microchip.com/downloads/jp/AppNotes/00001525B_JP.pdf
        SpO2 = -23.3 * (R - 0.4) + 112;
        // SpO2 = 1.5958422 * R * R + (-34.659662) * R + 112.6898759; // Maxim Integrated: Application Note 6845
        ESpO2 = FSpO2 * ESpO2 + (1.0 - FSpO2) * SpO2; //low pass filter
        //Serial.print(SpO2);
        //Serial.print(",");
        //Serial.println(ESpO2);
        sumredrms = 0.0;
        sumirrms = 0.0;
        i = 0;
        if (xQueueSend(fila, &ESpO2, (TickType_t)10000))
        {
          Serial.println("Fila Enviada");
          sensor = false;
        }
        else
          Serial.println("Fila não enviada...");
        break;
      }
      particleSensor.nextSample(); //We're finished with this sample so move to next sample

      //Serial.println(SpO2);
    }
  }
}

#endif