#ifndef _PHOTO_PERIOD_HEADER_
#define _PHOTO_PERIOD_HEADER_

#include <math.h>
#include <Arduino.h>
#include "timeduino.h"
#include "hardware_config.h"

#define ZENITH -.83
#define RESOLUTION 1

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

#endif
