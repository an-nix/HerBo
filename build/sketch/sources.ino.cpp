#include <Arduino.h>
#line 1 "c:\\project\\HerBo\\sources\\sources.ino"
#include <SPI.h>
#include <WiFiNINA.h>
#include <Wire.h>

#include "photo_period.h"
#include "timeduino.h"
#include "BH1750.h"

#include "hardware_config.h"



BH1750 lightMeter;
Light* l1 ;
Light* l2;

int _status;
int steps;
unsigned long next_millis;

#line 21 "c:\\project\\HerBo\\sources\\sources.ino"
void setup();
#line 78 "c:\\project\\HerBo\\sources\\sources.ino"
void loop();
#line 114 "c:\\project\\HerBo\\sources\\sources.ino"
int pairiod(uint32_t steps,uint32_t length);
#line 21 "c:\\project\\HerBo\\sources\\sources.ino"
void setup() 
{

    //Initialize Lights
    l1 = new Light(0,true);
    l2 = new Light(4,false);

  	//Initialize serial and wait for port to open:
  	Serial.begin(9600);
  	while (!Serial);// wait for serial port to connect. Needed for native USB port only

  	Serial.println("HerBo Start");

  	pinMode(LED_BUILTIN, OUTPUT);      // set the LED pin mode

    
    
  

    const byte seconds = 0;
    const byte minutes = 0;
    const byte hours = 16;


    const byte day = 21;
    const byte month = 3;
    const byte year = 20;


    set_time(get_epoch(year,month,day,hours,minutes,seconds));
    
    //rtc.attachInterrupt(sunrise);
    
    digitalWrite(LED_2_PIN, HIGH);


  // Initialize the I2C bus (BH1750 library doesn't do this automatically)
  // On esp8266 devices you can select SCL and SDA pins using Wire.begin(D4, D3);
  Wire.begin();

  lightMeter.begin();
  Serial.println(F("BH1750 Test"));

  //Global Variable init 
  next_millis = 0;
  _status = 0;
  steps = 0;
  
  //Set PWM Resolution on 10bits;
  #ifdef ADC_10_BITS
    analogWriteResolution(10);
  #else 
    analogWriteResolution(8);
  #endif
}


void loop() 
{

  switch(_status)
  {
    //Initializing
    //Je sais pas ou je suis, quand je suis
    //Led Blinking
    case 0:
      unsigned long m = millis();
      if(m>next_millis)
        {

          steps++;
          int u = pairiod(steps,250);
          
          Serial.println(u);
          //analogWrite(LED_1_PIN, 1023 - u);
          //analogWrite(LED_2_PIN, u);
          l1->dim(u);
          l2->dim(u);
          next_millis += 250 * RESOLUTION;
        }

      break;
  }

  #ifdef USE_MILLI
    nop();
  #endif
  
  //photoperiod();


}

int pairiod(uint32_t steps,uint32_t length)
{
  
  double dd = sin(((2 * PI * steps * RESOLUTION) / length) + (3*PI / 2 ));
  int ret = floor(1024*dd + 1024);
  #ifdef ADC_10_BITS
    ret = map(ret,0,256,ADC_LOWER_BOUND,ADC_UPPER_BOUND);
  #else
    ret = map(ret,0,256,ADC_LOWER_BOUND,ADC_UPPER_BOUND);
  #endif

  return(ret);
}








