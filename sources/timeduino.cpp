#include "timeduino.h"


#ifdef RTC
    RTCZero rtc;    
#else

#endif


    uint32_t alarm_epoch;
    void (*alarm_callback)();


uint32_t get_epoch(int year, byte month, byte day, byte hour, byte minute, byte second) {
    struct tm t;
    time_t t_of_day;

    t.tm_year = year - 1900;  // Year - 1900
    t.tm_mon = month-1;           // Month, where 0 = jan
    t.tm_mday = day;          // Day of the month
    t.tm_hour = hour;
    t.tm_min = minute;
    t.tm_sec = second;
    t.tm_isdst = -1;        // Is DST on? 1 = yes, 0 = no, -1 = unknown
    return(mktime(&t));

}

void init_time_RTCZero()
{
    rtc.begin(); // initialize RTC
}
void init_time_millis()
{

}

uint32_t get_time_RTCZero()
{
  return rtc.getEpoch();
}

uint32_t get_time_millis()
{
  return 1000;
}

void set_time_RTCZero(uint32_t timestamp)
{
    rtc.setEpoch(timestamp); // Jan 1, 2016
}
void set_time_millis(uint32_t timestamp)
{

}

int alarm_wo_rtc()
{

}

void init_alarm_RTCZero(void (* func)())
{
   rtc.attachInterrupt(func);
}

void init_alarm_millis(void (* func)())
{
    alarm_callback = func;
}

uint32_t get_midnight(uint32_t epoch,bool next_day)
{
    uint32_t since_mid = epoch % 86400;
    uint32_t _t = epoch - since_mid;
    if(next_day)
        _t += 86400;
    Serial.println("Midnight");
    Serial.println((long)_t);
    return _t;
}