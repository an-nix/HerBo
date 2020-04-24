#ifndef _TIMEDUINO_HEADER_
#define _TIMEDUINO_HEADER_

#include <time.h>
#include <RTCZero.h>

#define EPOCH_YEAR_START 1900
//#define EPOCH_DEFAULT 1590472800
#define EPOCH_DEFAULT 1590476160
//#define EPOCH_DEFAULT 1587324229

#if BOARD==SAMD_MKR1000
    #define USE_RTC
#else
    #define USE_MILLI
#endif 

#ifdef USE_RTC
    #define get_time()      get_time_RTCZero()
    #define init_time()     init_time_RTCZero()
    #define set_time(t)     set_time_RTCZero(t)
    #define init_alarm(t)   init_alarm_RTCZero(t)
    #define set_alarm(t)    set_alarm_RTCZero(t)    
    extern RTCZero rtc;
#else
    #define get_time()      get_time_millis()
    #define init_time()     init_time_millis()
    #define set_time(t)     set_time_millis(t)
    #define init_alarm(t)   init_alarm_millis(t)
    #define set_alarm(t)    set_alarm_millis(t) 
    extern uint32_t alarm_epoch;
#endif


uint32_t get_midnight(uint32_t,bool);

uint32_t get_epoch(int,byte,byte,byte,byte,byte);


//RTC Zero Lib
void init_time_RTCZero();
uint32_t get_time_RTCZero();
void set_time_RTCZero(uint32_t);
void init_alarm_RTCZero(void (*)());
void set_alarm_RTCZero(uint32_t); 

//Milli
void init_time_millis();
uint32_t get_time_millis();
void set_time_millis(uint32_t);
void init_alarm_millis(void (*)());
void set_alarm_millis(uint32_t);

int alarm_wo_rtc();
int get_year(uint32_t);


#endif