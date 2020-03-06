# 1 "c:\\project\\HerBo\\sources\\sources.ino"
# 2 "c:\\project\\HerBo\\sources\\sources.ino" 2
# 3 "c:\\project\\HerBo\\sources\\sources.ino" 2
# 4 "c:\\project\\HerBo\\sources\\sources.ino" 2

# 6 "c:\\project\\HerBo\\sources\\sources.ino" 2








# 13 "c:\\project\\HerBo\\sources\\sources.ino"
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
    pinMode(A4, (0x1)); // set the LED pin mode
   pinMode(14,(0x2));

    rtc.begin(); // initialize RTC

    //rtc.setEpoch(1451606400); // Jan 1, 2016


/* Change these values to set the current initial time */
    const byte seconds = 0;
    const byte minutes = 0;
    const byte hours = 16;

/* Change these values to set the current initial date */
    const byte day = 21;
    const byte month = 3;
    const byte year = 20;

  // you can use also
    rtc.setTime(hours, minutes, seconds);
    rtc.setDate(day, month, year);

    photoperiod_step = 0;
    photoperiod_max_steps = get_steps(0,1,0);
    rtc.attachInterrupt(sunrise);
    printSunrise();
    printSunset();

}


void loop()
{
  //Serial.println(rtc.getEpoch());
  photoperiod();
}

void sunrise()
{
  photoperiod_step = 0;
}

//Get number of Steps for the photo period
uint32_t get_steps(int hours, int minutes, int seconds)
{
  float f = (seconds + 60*minutes + 3600*hours) / 30;
  SerialUSB.println(floor(f));
  return floor(f);
}

void set_next_sunrise(){
    rtc.enableAlarm(rtc.MATCH_HHMMSS);
    rtc.setAlarmTime(rtc.getHours(),rtc.getMinutes() + 1,rtc.getSeconds());

}

void photoperiod()
{
  //Am I in day mode
  if(photoperiod_step != -1) {
    int epoch = rtc.getEpoch();

    //Sunrise
    if(photoperiod_step == 0) {
      SerialUSB.println("PhotoPeriod Start");
      //Turn On Light
      digitalWrite((6u),(0x1));
      digitalWrite(A4,(0x1));
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
      digitalWrite(A4,(0x0));
      set_next_sunrise();
    }

    else if(epoch >= photoperiod_next) {

      photoperiod_next = epoch + 30;
      SerialUSB.println(epoch);
      photoperiod_step++;
    }
  }
}


int get_day_of_the_year(int year,int month, int day) {


}

float calculateSunrise(bool rise,int year,int month,int day,float lat, float lng,int localOffset, int daylightSavings) {
    /*
    localOffset will be <0 for western hemisphere and >0 for eastern hemisphere
    daylightSavings should be 1 if it is in effect during the summer otherwise it should be 0
    */
    //1. first calculate the day of the year
    float N1 = floor(275 * month / 9);
    float N2 = floor((month + 9) / 12);
    float N3 = (1 + floor((year - 4 * floor(year / 4) + 2) / 3));
    float N = N1 - (N2 * N3) + day - 30;

    //2. convert the longitude to hour value and calculate an approximate time
    float lngHour = lng / 15.0;
    float t;
    if(rise)
    {
      t = N + ((6 - lngHour) / 24); //if rising time is desired:
    }
    else
    {
      t = N + ((18 - lngHour) / 24); //if setting time is desired:
    }

    //3. calculate the Sun's mean anomaly   
    float M = (0.9856 * t) - 3.289;

    //4. calculate the Sun's true longitude
    float L = fmod(M + (1.916 * sin((3.1415926535897932384626433832795/180)*M)) + (0.020 * sin(2 *(3.1415926535897932384626433832795/180) * M)) + 282.634,360.0);
    SerialUSB.println(L);

    //5a. calculate the Sun's right ascension      
    float RA = fmod(180/3.1415926535897932384626433832795*atan(0.91764 * tan((3.1415926535897932384626433832795/180)*L)),360.0);
    SerialUSB.println(RA);

    //5b. right ascension value needs to be in the same quadrant as L   
    float Lquadrant = floor( L/90) * 90;
    float RAquadrant = floor(RA/90) * 90;
    RA = RA + (Lquadrant - RAquadrant);

    //5c. right ascension value needs to be converted into hours   
    RA = RA / 15;

    //6. calculate the Sun's declination
    float sinDec = 0.39782 * sin((3.1415926535897932384626433832795/180)*L);
    float cosDec = cos(asin(sinDec));

    //7a. calculate the Sun's local hour angle
    float cosH = (sin((3.1415926535897932384626433832795/180)*-.83) - (sinDec * sin((3.1415926535897932384626433832795/180)*lat))) / (cosDec * cos((3.1415926535897932384626433832795/180)*lat));
    /*   
    if (cosH >  1) 
    the sun never rises on this location (on the specified date)
    if (cosH < -1)
    the sun never sets on this location (on the specified date)
    */

    //7b. finish calculating H and convert into hours
    float H;
    if(rise)
    {
      H = 360 - (180/3.1415926535897932384626433832795)*acos(cosH); //   if if rising time is desired:
    }
    else
    {
      H = (180/3.1415926535897932384626433832795)*acos(cosH); //   if setting time is desired:      
    }

    H = H / 15;

    //8. calculate local mean time of rising/setting      
    float T = H + RA - (0.06571 * t) - 6.622;

    //9. adjust back to UTC & convert UT value to local time zone of latitude/longitude
    float UT = fmod(T - lngHour,24.0) + localOffset + daylightSavings;
    while(UT < 0 | UT > 24)
    {
      if(UT < 0)
          UT += 24;
      else if (UT > 24)
          UT -= 24;
    }


    //10. 
    return UT;

    }

void printSunrise() {
    float localT = calculateSunrise(1,rtc.getYear(),rtc.getMonth(),rtc.getDay(),19.04,-98.2,-6,0);
    double hours;
    float minutes = modf(localT,&hours)*60;
    SerialUSB.print("Sunrise:");
    SerialUSB.print(hours);
    SerialUSB.print(":");
    SerialUSB.print(minutes);
    SerialUSB.println();

    }

void printSunset(){
    float localT = calculateSunrise(0,rtc.getYear(),rtc.getMonth(),rtc.getDay(),19.04,-98.2,-6,0);
    double hours;
    float minutes = modf(localT,&hours)*60;
    SerialUSB.print("Sunset:");
    SerialUSB.print(hours);
    SerialUSB.print(":");
    SerialUSB.print(minutes);
    SerialUSB.println();


}
