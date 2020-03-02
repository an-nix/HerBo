#include <Arduino.h>
#line 1 "c:\\project\\HerBo\\sources\\sources.ino"
#include <SPI.h>
#include <WiFiNINA.h>
#include <RTCZero.h>


#define CONFIG_PIN 14
#define RESOLUTION 30

void photoperiod_ramp(uint64_t start, uint64_t stop, uint8_t level);

/* Create an rtc object */
RTCZero rtc;

/******************************************************
 * PhotoPeriod
 ******************************************************/
int photoperiod_step;
int photoperiod_max_steps;
int photoperiod_next;


#line 22 "c:\\project\\HerBo\\sources\\sources.ino"
void setup();
#line 58 "c:\\project\\HerBo\\sources\\sources.ino"
void loop();
#line 65 "c:\\project\\HerBo\\sources\\sources.ino"
void print2digits(int number);
#line 73 "c:\\project\\HerBo\\sources\\sources.ino"
uint32_t get_steps(int hours, int minutes, int seconds);
#line 81 "c:\\project\\HerBo\\sources\\sources.ino"
void photoperiod_ramp();
#line 22 "c:\\project\\HerBo\\sources\\sources.ino"
void setup() 
{

  	//Initialize serial and wait for port to open:
  	Serial.begin(9600);
  	while (!Serial);// wait for serial port to connect. Needed for native USB port only

  	Serial.println("HerBo Start");

  	pinMode(LED_BUILTIN, OUTPUT);      // set the LED pin mode
  	pinMode(CONFIG_PIN,INPUT_PULLUP);  

    rtc.begin(); // initialize RTC

    //rtc.setEpoch(1451606400); // Jan 1, 2016
    
    
/* Change these values to set the current initial time */
    const byte seconds = 0;
    const byte minutes = 0;
    const byte hours = 16;

/* Change these values to set the current initial date */
    const byte day = 26;
    const byte month = 5;
    const byte year = 20;

  // you can use also
    rtc.setTime(hours, minutes, seconds);
    rtc.setDate(day, month, year);

    photoperiod_step = 0;
    photoperiod_max_steps = get_steps(0,1,0);
}


void loop() 
{
  //Serial.println(rtc.getEpoch());
  photoperiod_ramp();
}


void print2digits(int number) {
  if (number < 10) {
    Serial.print("0"); // print a 0 before if the number is < than 10
  }
  Serial.print(number);
}

//Get number of Steps for the photo period
uint32_t get_steps(int hours, int minutes, int seconds)
{
  float f = (seconds + 60*minutes + 3600*hours) / RESOLUTION;
  Serial.println(floor(f));
  return floor(f);
}


void photoperiod_ramp() 
{
  //Am I in day mode
  if(photoperiod_step != -1) {
    int epoch = rtc.getEpoch();
    
    //Sunrise
    if(photoperiod_step == 0) {
      Serial.println("PhotoPeriod Start");
      //Turn On Light
      digitalWrite(LED_BUILTIN,HIGH);
      //Set next alarm to resolution
      photoperiod_next = epoch + RESOLUTION;
      photoperiod_step++;
    }

    //Sunset
    else if(photoperiod_step > photoperiod_max_steps) {
      Serial.println("PhotoPeriod Stop");
      photoperiod_step=-1;
      //Turn Off Light
      digitalWrite(LED_BUILTIN,LOW);
      //Set Next alarm
    }

    else if(epoch >= photoperiod_next) {
      
      photoperiod_next = epoch + RESOLUTION;
      Serial.println(epoch);
      photoperiod_step++;
    }
  }
}




