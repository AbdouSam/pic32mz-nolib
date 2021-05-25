#ifndef INCLUDED_PIC32_TIMER_H
#define INCLUDED_PIC32_TIMER_H

typedef enum
{
  PIC32_TIMER_1,
  PIC32_TIMER_2,
  PIC32_TIMER_3,
  PIC32_TIMER_4,
  PIC32_TIMER_5,
  PIC32_TIMER_6,
  PIC32_TIMER_7,
  PIC32_TIMER_8,
  PIC32_TIMER_9,
  PIC32_TIMER_MAX,
}pic32_timer_t;

typedef enum
{
  TMR1_PRESCALE_1   = 0b00,
  TMR1_PRESCALE_8   = 0b01,
  TMR1_PRESCALE_64  = 0b10,
  TMR1_PRESCALE_256 = 0b11,
}timer1_prescaler_t;

/* timers from 2 to 9. */

typedef enum
{
  TMR_PRESCALE_1    = 0b000,
  TMR_PRESCALE_2    = 0b001,
  TMR_PRESCALE_4    = 0b010,
  TMR_PRESCALE_8    = 0b011,
  TMR_PRESCALE_16   = 0b100,
  TMR_PRESCALE_32   = 0b101,
  TMR_PRESCALE_64   = 0b110,
  TMR_PRESCALE_256  = 0b111,
  TMR_PRESCALE_MAX_INDX  = 8,
}timers_prescaler_t;

void timer_1_init(void);

int timer_init(pic32_timer_t timer_id, 
                uint32_t freq,
                uint8_t subp);

int timer_stop(pic32_timer_t timer_id);

#endif /* INCLUDED_PIC32_TIMER_H */
