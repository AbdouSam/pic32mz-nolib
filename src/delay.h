#ifndef MICROPY_INCLUDED_PIC32BIT_DELAY_H
#define MICROPY_INCLUDED_PIC32BIT_DELAY_H

#include <stdint.h>

/*
 * @brief generate a a blocking delay in us
 */
void delay_us(uint32_t us);

/*
 * @brief generate a a blocking delay in ms
 */
void delay_ms(uint32_t ms);


uint32_t readcoretimer(void);

void setcoretime(volatile uint32_t count);


#endif /* MICROPY_INCLUDED_PIC32BIT_DELAY_H */