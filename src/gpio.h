#ifndef MICROPY_INCLUDED_PIC32_GPIO_H
#define MICROPY_INCLUDED_PIC32_GPIO_H

#include <stdint.h>
#include <stdbool.h>

typedef enum
{
#if (PIC32_PIN_COUNT != 64)
  PIC32_PORTA,
#endif
  PIC32_PORTB,
  PIC32_PORTC,
  PIC32_PORTD,
  PIC32_PORTE,
  PIC32_PORTF,
  PIC32_PORTG,
#if ((PIC32_PIN_COUNT != 64) && (PIC32_PIN_COUNT != 100))
  PIC32_PORTH,
  PIC32_PORTJ,
#endif
#if ((PIC32_PIN_COUNT != 64) && (PIC32_PIN_COUNT != 100) && (PIC32_PIN_COUNT != 124))
  PIC32_PORTK,
#endif
  PIC32_PORT_MAX,
}pic32_gpio_port_t;

enum
{
  PIC32_GPIO_OUTPUT,
  PIC32_GPIO_INPUT,
};

typedef enum
{
#if (PIC32_PIN_COUNT != 64)
 pinA0 = 0,
 pinA1 = 1,
 pinA2 = 2,
 pinA3 = 3,
 pinA4 = 4,
 pinA5 = 5,
 pinA6 = 6,
 pinA7 = 7,
 pinA9 = 9,
 pinA10 = 10,
 pinA14 = 14,
 pinA15 = 15,
#endif

 pinB0 = 0,
 pinB1 = 1,
 pinB2 = 2,
 pinB3 = 3,
 pinB4 = 4,
 pinB5 = 5,
 pinB6 = 6,
 pinB7 = 7,
 pinB8 = 8,
 pinB9 = 9,
 pinB10 = 10,
 pinB11 = 11,
 pinB12 = 12,
 pinB13 = 13,
 pinB14 = 14,
 pinB15 = 15,

#if (PIC32_PIN_COUNT != 64)
 pinC1 = 1,
 pinC2 = 2,
 pinC3 = 3,
 pinC4 = 4,
#endif
 pinC12 = 12,
 pinC13 = 13,
 pinC14 = 14,
 pinC15 = 15,

 pinD0 = 0,
 pinD1 = 1,
 pinD2 = 2,
 pinD3 = 3,
 pinD4 = 4,
 pinD5 = 5,
#if ((PIC32_PIN_COUNT != 64) && (PIC32_PIN_COUNT != 100))
 pinD6  = 6 ,
 pinD7  = 7 ,
#endif
 pinD9  = 9 ,
 pinD10 = 10,
 pinD11 = 11,
#if (PIC32_PIN_COUNT != 64)
 pinD12 = 12,
 pinD13 = 13,
 pinD14 = 14,
 pinD15 = 15,
#endif

 pinE0 = 0,
 pinE1 = 1,
 pinE2 = 2,
 pinE3 = 3,
 pinE4 = 4,
 pinE5 = 5,
 pinE6 = 6,
 pinE7 = 7,
#if (PIC32_PIN_COUNT != 64)
 pinE8 = 8,
 pinE9 = 9,
#endif

 pinF0 = 0,
 pinF1 = 1,
#if (PIC32_PIN_COUNT != 64)
 pinF2 = 2,
#endif
 pinF3 = 3,
 pinF4 = 4,
 pinF5 = 5,
 pinF8 = 8,
#if (PIC32_PIN_COUNT != 64)
 pinF12 = 12,
 pinF13 = 13,
#endif

#if (PIC32_PIN_COUNT != 64)
 pinG0 = 0,
 pinG1 = 1,
#endif
 pinG6 = 6,
 pinG7 = 7,
 pinG8 = 8,
 pinG9 = 9,
#if (PIC32_PIN_COUNT != 64)
 pinG12 = 12,
 pinG13 = 13,
 pinG14 = 14,
 pinG15 = 15,
#endif

#if ((PIC32_PIN_COUNT != 64) && (PIC32_PIN_COUNT != 100))
 pinH0 = 0,
 pinH1 = 1,
#if (PIC32_PIN_COUNT != 124)
 pinH2 = 2,
 pinH3 = 3,
#endif
 pinH4 = 4,
 pinH5 = 5,
 pinH6 = 6,
#if (PIC32_PIN_COUNT != 124)
 pinH7 = 7,
#endif
 pinH8 = 8,
 pinH9 = 9,
 pinH10 = 10,
#if (PIC32_PIN_COUNT != 124)
 pinH11 = 11,
#endif
 pinH12 = 12,
 pinH13 = 13,
#if (PIC32_PIN_COUNT != 124)
 pinH14 = 14,
 pinH15 = 15,
#endif

 pinJ0 = 0,
 pinJ1 = 1,
 pinJ2 = 2,
#if (PIC32_PIN_COUNT != 124)
 pinJ3 = 3,
#endif
 pinJ4 = 4,
#if (PIC32_PIN_COUNT != 124)
 pinJ5 = 5,
 pinJ6 = 6,
 pinJ7 = 7,
#endif
 pinJ8 = 8,
 pinJ9 = 9,
#if (PIC32_PIN_COUNT != 124)
 pinJ10 = 10,
#endif
 pinJ11 = 11,
#if (PIC32_PIN_COUNT != 124)
 pinJ12 = 12,
 pinJ13 = 13,
 pinJ14 = 14,
 pinJ15 = 15,
#endif
#if (PIC32_PIN_COUNT != 124)
 pinK0 = 0,
 pinK1 = 1,
 pinK2 = 2,
 pinK3 = 3,
 pinK4 = 4,
 pinK5 = 5,
 pinK6 = 6,
 pinK7 = 7,
#endif // 124
#endif // 64 && 100
}pic32_pin_t;

void gpio_init(void);
bool gpio_state_get(pic32_gpio_port_t port, pic32_pin_t nbr);
void gpio_state_set(pic32_gpio_port_t port, pic32_pin_t nbr, bool state);
void gpio_input_set(pic32_gpio_port_t port, pic32_pin_t nbr);
void gpio_output_set(pic32_gpio_port_t port, pic32_pin_t nbr);
void gpio_state_toggle(pic32_gpio_port_t port, pic32_pin_t nbr);

#endif // MICROPY_INCLUDED_PIC32_GPIO_H 