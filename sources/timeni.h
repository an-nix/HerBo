#ifndef _TIMENI_HEADER_
#define _TIMENI_HEADER_

#include <RTCZero.h>

class iTimeNi {

    public :
        virtual uint32_t get() = 0;
        virtual void set(uint32_t) = 0;
        virtual void initAlarm(void (*callback)()) = 0;
        virtual void setAlarm(uint32_t epoch) = 0;
        virtual void loop() = 0;

};

class TimeNiMillis : protected iTimeNi {
    
};

class TimeNiRTCZero : protected iTimeNi
{

    private:
        RTCZero __rtc;

    public:
        TimeNiRTCZero();
        uint32_t get();
        void set(uint32_t epoch);
        void initAlarm(void (*callback)());
        void setAlarm(uint32_t epoch);
        void loop() {};

};

#endif