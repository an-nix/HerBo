#ifndef _HW_CONFIG_HEADER
#define _HW_CONFIG_HEADER

#define LED_1_PIN 0
#define LED_1_INVERTED 1

#define LED_2_PIN 5
#define LED_2_INVERTED 0

#define ADC_10_BITS
#define ADC_LOWER_BOUND 100
#define ADC_UPPER_BOUND 1023


//
#define LOGGING_WAITING() led_drive(255,0,128)
//STARTING
#define LOGGING_STARTING() led_drive(0xFF,0xA5,0x00)
//ERROR 
#define LOGGING_ERROR() led_drive(0xFF,0x00,0x00)
//RUNNING 
#define LOGGING_READY() led_drive(0x00,0xFF,0x00)




#endif
