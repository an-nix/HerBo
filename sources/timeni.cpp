#include "./timeni.h"



TimeNiRTCZero::TimeNiRTCZero() {
    this->__rtc.begin();
}

uint32_t TimeNiRTCZero::get() {
    return(this->__rtc.getEpoch());
}

void TimeNiRTCZero::set(uint32_t epoch) {
    this->__rtc.setEpoch(epoch);
}

void TimeNiRTCZero::initAlarm(void (*callback)()){
    this->__rtc.attachInterrupt(callback);
    this->__rtc.enableAlarm(this->__rtc.MATCH_YYMMDDHHMMSS);
}

void TimeNiRTCZero::setAlarm(uint32_t epoch) {
    this->__rtc.setAlarmEpoch(epoch);
}