#ifndef INCLUDED_APP_H
#define INCLUDED_APP_H

#define LED_YELLOW  pinH6
#define LED_RED     pinB13
#define LED_ORANGE  pinB12
#define LED_GREEN   pinA1

#define RS485_1_RW  pinA10
#define RS485_2_RW  pinA9

#define RELAY_OUT1  pinE9 
#define RELAY_OUT2  pinE8 
#define RELAY_OUT3  pinA0 
#define RELAY_OUT4  pinG8 
#define RELAY_OUT5  pinG7 
#define RELAY_OUT6  pinG6 
#define RELAY_OUT7  pinG15
#define RELAY_OUT8  pinH2 
#define RELAY_OUT9  pinH1 
#define RELAY_OUT10 pinH0 
#define RELAY_OUT11 pinB2 
#define RELAY_OUT12 pinB3 
#define RELAY_OUT13 pinB4 
#define RELAY_OUT14 pinB5 
#define RELAY_OUT15 pinH3 
#define RELAY_OUT16 pinB11

#define DIG_IN1  pinK7 
#define DIG_IN2  pinD7 
#define DIG_IN3  pinD5 
#define DIG_IN4  pinJ6 
#define DIG_IN5  pinJ3 
#define DIG_IN6  pinJ2 
#define DIG_IN7  pinJ0 
#define DIG_IN8  pinC13
#define DIG_IN9  pinF13
#define DIG_IN10 pinD4 
#define DIG_IN11 pinD10
#define DIG_IN12 pinD15
#define DIG_IN13 pinF8 
#define DIG_IN14 pinD0 
#define DIG_IN15 pinA14
#define DIG_IN16 pinA15
/**
 * @brief app_init is called from the main before the infinite loop
 */
int app_init(void);

/**
 * @brief app_task is called in an infinite loop from the main
 */
void app_task(void);

#endif /* INCLUDED_APP_H */