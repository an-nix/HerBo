#ifndef _PHOTO_PERIOD_HEADER_
#define _PHOTO_PERIOD_HEADER_

#include <math.h>
#include <Arduino.h>
#include "timeduino.h"
#include "hardware_config.h"

#define MAX_LED_GROUP 2

#define ZENITH -.83
#define RESOLUTION 1


#define SYNC_UP_DELTA 0
#define SYNC_DOWN_DELTA 0

extern int photoperiod_step;
extern int photoperiod_max_steps;
extern int photoperiod_next;

void photoperiod_init();

void photoperiod();

void sunrise();
uint32_t get_steps(int, int, int);
void set_next_sunrise();
float calculateSunrise(bool,int,int,int,float, float,int, int);

int luminosity_value(uint32_t, uint32_t);

class Light
{

    private:
        int pin;
        bool inverted;
    
    public:
        Light(int,bool);
        void toggle(void);
        void toggle(bool);
        void dim(int value);
};

typedef struct suntime
{
    uint32_t sunrise;
    uint32_t duration;

} stime;

class PhotoPeriod
{
    private:

        Light** _lights;
        int _iter;
        float _long;
        float _lat;
        int _localOffset;

        uint32_t _step;
        uint32_t _max_steps;
        uint32_t _nextEpoch;
        suntime _stime;

    public:

        PhotoPeriod(float longitude,float latitude,int localOffset);
        int add_light(Light*);
        int add_light(int,bool);
        uint32_t get_next_sunrise(bool rise,uint32_t epoch, bool next_day);
        void sync(uint32_t epoch);
        void loop(void);
        int get_luminosity(uint32_t);
        void init_period();
        uint32_t get_sunrise_time();

};





#endif
