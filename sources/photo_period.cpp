#include "photo_period.h"



Light::Light(int pin, bool inverted) {
  this->pin = pin;
  this->inverted = inverted;

  pinMode(this->pin, OUTPUT);

  this->toggle(false);
}

void Light::toggle(void) {
    digitalWrite(this->pin,!digitalRead(this->pin));
}

void Light::toggle(bool value) {
    //Turn Light Off
  if(this->inverted)
    digitalWrite(this->pin,!value);
  else
    digitalWrite(this->pin,value);
}

void Light::dim(int value) {
    //Turn Light Off
  if(this->inverted)
    #ifdef ADC_10_BITS
      analogWrite(this->pin,1023-value);
    #else 
      analogWrite(this->pin,255-value);
    #endif
  else
    analogWrite(this->pin,value);

}


PhotoPeriod::PhotoPeriod(float longitude, float latitude, int localOffset) {
  this->_lights = new Light*[MAX_LED_GROUP];
  this->_long = longitude;
  this->_lat = latitude;
  this->_localOffset = localOffset;
  this->_step = 0;
  this->_max_steps = 0;
  this->_iter = 0;
  this->_nextEpoch = 0;
  //init_alarm(init_period);
  
 
}

//Add an existing Light object
int PhotoPeriod::add_light(Light* light) {
  //Control if Light list is full
  if(this->_iter == MAX_LED_GROUP)
    return 1; 
  else {
    this->_lights[this->_iter] = light;
    this->_iter++;
    return 0;
  }
}

//Add a new Light
int PhotoPeriod::add_light(int pin, bool inverted) {
  //Control if Light list is full
  if(this->_iter == MAX_LED_GROUP)
    return 1; 
  else {
    this->_lights[this->_iter] = new Light(pin, inverted);
    this->_iter++;
    return 0;
  }

}

uint32_t PhotoPeriod::get_next_sunrise(bool rise,uint32_t epoch, bool next_day) {

  time_t t_ = get_midnight(epoch,next_day);
  struct tm date;
  date = *localtime(&t_);

  //1. first calculate the day of the year
  float N = date.tm_yday + 1;

  //2. convert the longitude to hour value and calculate an approximate time
  float lngHour = this->_long / 15.0;
  float t;
  if(rise) 
  {      
    t = N + ((6 - lngHour) / 24);   //if rising time is desired:
  }
  else
  {
    t = N + ((18 - lngHour) / 24);   //if setting time is desired:
  }

    //3. calculate the Sun's mean anomaly   
    float M = (0.9856 * t) - 3.289;

    //4. calculate the Sun's true longitude
    float L = fmod(M + (1.916 * sin((PI/180)*M)) + (0.020 * sin(2 *(PI/180) * M)) + 282.634,360.0);
    while(L < 0 || L > 360)
    {
      if(L < 0)
          L += 360;
      else if (L > 360)
          L -= 360;
    }

    //5a. calculate the Sun's right ascension      
    float RA = fmod(180/PI*atan(0.91764 * tan((PI/180)*L)),360.0);
    while(RA < 0 || RA > 360)
    {
      if(RA < 0)
          RA += 360;
      else if (RA > 360)
          RA -= 360;
    }

    //5b. right ascension value needs to be in the same quadrant as L   
    float Lquadrant  = floor( L/90) * 90;
    float RAquadrant = floor(RA/90) * 90;
    RA = RA + (Lquadrant - RAquadrant);

    //5c. right ascension value needs to be converted into hours   
    RA = RA / 15;

    //6. calculate the Sun's declination
    float sinDec = 0.39782 * sin((PI/180)*L);
    float cosDec = cos(asin(sinDec));

    //7a. calculate the Sun's local hour angle
    float cosH = (sin((PI/180)*ZENITH) - (sinDec * sin((PI/180)*this->_lat))) / (cosDec * cos((PI/180)*this->_lat));
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
      H = 360 - (180/PI)*acos(cosH);   //   if if rising time is desired:
    }
    else
    {
      H = (180/PI)*acos(cosH); //   if setting time is desired:      
    }
    
    H = H / 15;

    //8. calculate local mean time of rising/setting      
    float T = H + RA - (0.06571 * t) - 6.622;

    //9. adjust back to UTC & convert UT value to local time zone of latitude/longitude
    float UT = fmod(T - lngHour,24.0) + this->_localOffset;
    while(UT < 0 || UT > 24)
    {
      if(UT < 0)
          UT += 24;
      else if (UT > 24)
          UT -= 24;
    }


 
    //Serial.println("Sunrise Mid");
    //Serial.println((long)t_ + (UT*3600));
    return t_ + (UT*3600);

}


void PhotoPeriod::sync(uint32_t epoch) {

  //Get next sunset
  uint32_t sunset = this->get_next_sunrise(false,epoch,false);

  //If the next sunset is in the past, recalculate for the next day
  if(epoch > sunset - SYNC_DOWN_DELTA)
  {
    this->_stime.sunrise = this->get_next_sunrise(true,epoch,true);
    sunset = this->get_next_sunrise(false,epoch,true);
  }
  else
  {
    this->_stime.sunrise = this->get_next_sunrise(true,epoch,false);
    

    //Next Sunrise is coming
    if(this->_stime.sunrise > epoch + SYNC_UP_DELTA)
    {
    //Set alarm
    Serial.print(this->_stime.sunrise - epoch);
    Serial.print(" second to wait");
    set_alarm(this->_stime.sunrise);
    }
    //Work In Progress (Sunset is coming)
    else if(epoch  + SYNC_DOWN_DELTA < sunset)
    {
      //Get Remaining Step
      Serial.println("Work in Progress");
      this->_step = floor((epoch - this->_stime.sunrise) / RESOLUTION);
      this->_nextEpoch = epoch + RESOLUTION;
      Serial.print("Step: ");
      Serial.println(this->_step);
    }     

  }
    Serial.print("Sunset : ");
    Serial.println(sunset);
    Serial.print("Sunrise : ");
    Serial.println(this->_stime.sunrise);
    Serial.print("Duration : ");
    Serial.println(sunset - this->_stime.sunrise);
    this->_stime.duration = sunset - this->_stime.sunrise;
    this->_max_steps = floor(this->_stime.duration / (RESOLUTION*100));
    Serial.print("Max Step: ");
    Serial.println(this->_max_steps);

}

void PhotoPeriod::loop(void)
{

  // Day mode?
  if(this->_nextEpoch > 0)
  {

    uint32_t current_epoch = get_time();

    if( current_epoch >= this->_nextEpoch  )
    {
      if(this->_step >= this->_max_steps)
      {
        this->_nextEpoch = 0;
        this->_step = 0;
        //go in night mode
      }
      else
      {
        //////////////////
        //DRIFT DETECTOR
        int drift = current_epoch - this->_nextEpoch;
        Serial.println(this->_step);
        //Serial.println(current_epoch);
        //Serial.println(this->_nextEpoch);
        //Serial.println(drift);  
        if(drift >= RESOLUTION)
        {
          this->_step += floor(drift/RESOLUTION) ;           
        }
        /////////////////
        uint32_t luminosity = this->get_luminosity(this->_step);
        Serial.println(luminosity);
        for(int i = 0;i<this->_iter;i++)
          this->_lights[i]->dim(luminosity); 
        this->_nextEpoch = current_epoch + RESOLUTION;
        this->_step++;
      }
    }
  }

  
   
}

int PhotoPeriod::get_luminosity(uint32_t steps)
{
  
  double dd = sin(((2 * PI * steps ) / this->_max_steps) + (3*PI / 2 ));
  int ret = floor(512*dd + 512);
  #ifdef ADC_10_BITS
    ret = map(ret,0,1024,ADC_LOWER_BOUND,ADC_UPPER_BOUND);
  #else
    ret = map(ret,0,1024,ADC_LOWER_BOUND,ADC_UPPER_BOUND);
  #endif

  return(ret);
}

void PhotoPeriod::init_period() {

    this->_nextEpoch  = get_time();
    this->_step = 0;   
}

uint32_t PhotoPeriod::get_sunrise_time() {

  return(this->_stime.sunrise);
}


