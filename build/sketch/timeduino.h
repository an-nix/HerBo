#ifndef _TIMEDUINO_HEADER_
#define _TIMEDUINO_HEADER_

#include <time.h>
#include <RTCZero.h>

#define EPOCH_YEAR_START 1900

#if BOARD==SAMD_MKR1000
    #define USE_RTC
#else
    #define USE_MILLI
#endif 

#ifdef USE_RTC
    #define get_time()      get_time_RTCZero()
    #define init_time()     init_time_RTCZero()
    #define set_time(t)      set_time_RTCZero(t)
    extern RTCZero rtc;
#else
    #define get_time()      get_time_millis()
    #define init_time()     init_time_millis()
    #define set_time(t)      set_time_millis(t)
    extern uint32_t alarm_epoch;
#endif



uint32_t get_epoch(byte,byte,byte,byte,byte,byte);


//RTC Zero Lib
void init_time_RTCZero();
uint32_t get_time_RTCZero();
void set_time_RTCZero(uint32_t);
void init_alarm_RTCZero(void (*)());

//Milli
void init_time_millis();
uint32_t get_time_millis();
void set_time_millis(uint32_t);
void init_alarm_millis(void (*)());

int alarm_wo_rtc();

#endif