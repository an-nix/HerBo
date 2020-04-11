#include <Arduino.h>
#line 1 "c:\\project\\HerBo\\sources\\sources.ino"
#include <SPI.h>
#include <WiFiNINA.h>
#include <Wire.h>

#include "photo_period.h"
#include "timeduino.h"
#include "BH1750.h"
#include "wifi.h"

#include "hardware_config.h"

#define STATUS_TIME		1000
#define RECONNECT_TIME 	10000

  Light *l1,*l2;
  //Light* l2;
  PhotoPeriod* ph;
  WiBo* w;


  int wifi_last_status_mil;
  int wifi_last_reconnect_mil;


#line 25 "c:\\project\\HerBo\\sources\\sources.ino"
void setup();
#line 68 "c:\\project\\HerBo\\sources\\sources.ino"
void loop();
#line 25 "c:\\project\\HerBo\\sources\\sources.ino"
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

  w = new WiBo("annix_home","welcome @t h0me");

  int wifi_last_status_mil = 0;
  int wifi_last_reconnect_mil = 0;

  w->connect_ap();
  w->update_status();

   

}


void loop() {


  int current_millis = millis();

  if((current_millis > wifi_last_reconnect_mil + RECONNECT_TIME) && 
        !(w->is_connected()))
  {
    w->connect_ap();
    wifi_last_reconnect_mil = current_millis;
  }

  if(current_millis > wifi_last_status_mil + STATUS_TIME)
  {
    w->update_status();
    Serial.println(current_millis - (wifi_last_status_mil + STATUS_TIME));
    wifi_last_status_mil = current_millis;
  }



  
  //update_led_status(w->_status);
  //photoperiod();
}














