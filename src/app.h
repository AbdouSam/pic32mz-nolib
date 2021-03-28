#ifndef APP_H
#define APP_H

#define LED_YELLOW  pinH6
#define LED_RED     pinB13
#define LED_ORANGE  pinB12
#define LED_GREEN   pinA1

#define RS485_RW    pinA10
/**
 * @brief app_init is called from the main before the infinite loop
 */
int app_init(void);

/**
 * @brief app_task is called in an infinite loop from the main
 */
void app_task(void);

#endif /* APP_H */