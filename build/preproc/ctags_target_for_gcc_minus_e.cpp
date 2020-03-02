# 1 "c:\\project\\HerBo\\sources\\sources.ino"
# 2 "c:\\project\\HerBo\\sources\\sources.ino" 2
# 3 "c:\\project\\HerBo\\sources\\sources.ino" 2
# 4 "c:\\project\\HerBo\\sources\\sources.ino" 2





void photoperiod_ramp(uint64_t start, uint64_t stop, uint8_t level);

/* Create an rtc object */
RTCZero rtc;

/******************************************************
 * PhotoPeriod
 ******************************************************/
int photoperiod_step;
int photoperiod_max_steps;
int photoperiod_next;


void setup()
{

   //Initialize serial and wait for port to open:
   SerialUSB.begin(9600);
   while (!SerialUSB);// wait for serial port to connect. Needed for native USB port only

   SerialUSB.println("HerBo Start");

   pinMode((6u), (0x1)); // set the LED pin mode
   pinMode(14,(0x2));

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
    SerialUSB.print("0"); // print a 0 before if the number is < than 10
  }
  SerialUSB.print(number);
}

//Get number of Steps for the photo period
uint32_t get_steps(int hours, int minutes, int seconds)
{
  float f = (seconds + 60*minutes + 3600*hours) / 30;
  SerialUSB.println(floor(f));
  return floor(f);
}


void photoperiod_ramp()
{
  //Am I in day mode
  if(photoperiod_step != -1) {
    int epoch = rtc.getEpoch();

    //Sunrise
    if(photoperiod_step == 0) {
      SerialUSB.println("PhotoPeriod Start");
      //Turn On Light
      digitalWrite((6u),(0x1));
      //Set next alarm to resolution
      photoperiod_next = epoch + 30;
      photoperiod_step++;
    }

    //Sunset
    else if(photoperiod_step > photoperiod_max_steps) {
      SerialUSB.println("PhotoPeriod Stop");
      photoperiod_step=-1;
      //Turn Off Light
      digitalWrite((6u),(0x0));
      //Set Next alarm
    }

    else if(epoch >= photoperiod_next) {

      photoperiod_next = epoch + 30;
      SerialUSB.println(epoch);
      photoperiod_step++;
    }
  }
}
