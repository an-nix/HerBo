#include <SPI.h>
#include <WiFiNINA.h>
#include <Wire.h>

#include "photo_period.h"
#include "timeduino.h"
#include "BH1750.h"

#include "hardware_config.h"


  Light* l1;
  Light* l2;
  PhotoPeriod* ph;

  const byte seconds = 0;
  const byte minutes = 50;
  const byte hours = 6;

  const byte day = 26;
  const byte month = 5;
  const int year = 2020;

void setup() 
{

  //Initialize serial and wait for port to open:
  Serial.begin(9600);
  while (!Serial);// wait for serial port to connect. Needed for native USB port only

  Serial.println("HerBo Start");

  pinMode(LED_BUILTIN, OUTPUT);      // set the LED pin mode

  //Set PWM Resolution on 10bits;
  #ifdef ADC_10_BITS
    analogWriteResolution(10);
  #else 
    analogWriteResolution(8);
  #endif

 //Initialize Lights
  l1 = new Light(0,true);
  l2 = new Light(4,false);

  ph = new PhotoPeriod(-98.183334,19.033333,-5);
  ph->add_light(l1);
  ph->add_light(l2);

  init_time();
  set_time(EPOCH_DEFAULT);
  ph->sync(EPOCH_DEFAULT);
  //Try 
}


void loop() {
  //photoperiod();
}






