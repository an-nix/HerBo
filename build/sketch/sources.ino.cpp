#include <Arduino.h>
#line 1 "c:\\project\\HerBo\\sources\\sources.ino"
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

#line 24 "c:\\project\\HerBo\\sources\\sources.ino"
void setup();
#line 66 "c:\\project\\HerBo\\sources\\sources.ino"
void loop();
#line 71 "c:\\project\\HerBo\\sources\\sources.ino"
void sync_sun(time_t epoch);
#line 24 "c:\\project\\HerBo\\sources\\sources.ino"
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


 //set_time(get_epoch(year,month,day,hours,minutes,seconds));

 //Initialize Lights
  l1 = new Light(0,true);
  l2 = new Light(4,false);

  ph = new PhotoPeriod(-98.183334,19.033333,-5);
  ph->add_light(l1);
  ph->add_light(l2);

  //Serial.println("Get Next Sunrise");
  //uint32_t ep = get_epoch(year,month,day,0,0,0) + (ph->get_next_sunrise(true,year,month,day,-7,1)*3600);
  //Serial.println(ep);

  init_time();
  set_time(EPOCH_DEFAULT);
  ph->sync(EPOCH_DEFAULT);
  //Try
  

}


void loop() {
  //photoperiod();

}

void sync_sun(time_t epoch)
{
  

  ph->get_next_sunrise(true,epoch,false);
  ph->get_next_sunrise(true,epoch,true);
  //sync time
  //get next Sunrise
  //is next sunrise passed?
  //is next sunset passed
  //get next sunrise and wait


}






