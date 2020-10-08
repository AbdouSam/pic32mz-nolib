#ifndef PIC32MZ_TIMER_H
#define PIC32MZ_TIMER_H

enum
{
  TIMER1,
  TIMER2,
  TIMER3,
  TIMER4,
  TIMER5,
  TIMER6,
  TIMER7,
  TIMER8,
  TIMER9,

};

typedef enum
{
  TMR1_PRESCALE_1   = 0b00,
  TMR1_PRESCALE_8   = 0b01,
  TMR1_PRESCALE_64  = 0b10,
  TMR1_PRESCALE_256 = 0b11,
}timer1_prescaler_t;

// timers from 2 to 9.

typedef enum
{
  TMR_PRESCALE_1   = 0b000,
  TMR_PRESCALE_2   = 0b001,
  TMR_PRESCALE_4   = 0b010,
  TMR_PRESCALE_8   = 0b011,
  TMR_PRESCALE_16  = 0b100,
  TMR_PRESCALE_32  = 0b101,
  TMR_PRESCALE_64  = 0b110,
  TMR_PRESCALE_256 = 0b111,
}timers_prescaler_t;



void init_timer1(uint32_t freq, timer1_prescaler_t prescaler, uint8_t sub_periority);

#if (TIMER_2_ENABLE == 1)
void init_timer2(uint32_t freq, timers_prescaler_t prescaler, uint8_t sub_periority);
#endif

#if (TIMER_3_ENABLE == 1)
void init_timer3(uint32_t freq, timers_prescaler_t prescaler, uint8_t sub_periority);
#endif

#if (TIMER_4_ENABLE == 1)
void init_timer4(uint32_t freq, timers_prescaler_t prescaler, uint8_t sub_periority);
#endif

#if (TIMER_5_ENABLE == 1)
void init_timer5(uint32_t freq, timers_prescaler_t prescaler, uint8_t sub_periority);
#endif

#if (TIMER_6_ENABLE == 1)
void init_timer6(uint32_t freq, timers_prescaler_t prescaler, uint8_t sub_periority);
#endif

#if (TIMER_7_ENABLE == 1)
void init_timer7(uint32_t freq, timers_prescaler_t prescaler, uint8_t sub_periority);
#endif

#if (TIMER_8_ENABLE == 1)
void init_timer8(uint32_t freq, timers_prescaler_t prescaler, uint8_t sub_periority);
#endif

#if (TIMER_9_ENABLE == 1)
void init_timer9(uint32_t freq, timers_prescaler_t prescaler, uint8_t sub_periority);
#endif


#endif // PIC32MZ_TIMER_H
