
/* pic32mz has 9 timers */

/*6 uarts*/
#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>

#define SYS_FREQ 200000000 // 200Mhz

#define PERPHERAL1_CLK_DIV (1)

/* UART, SPI, I2C, PMP */
#define PERPHERAL2_CLK_DIV (1)

/* For timers, comparator, output compare an input capture. */
#define PERPHERAL3_CLK_DIV (4)

/* For GPIOs*/
#define PERPHERAL4_CLK_DIV (1)

/**/
#define PERPHERAL5_CLK_DIV (1)

/* Deadman timer */
#define PERPHERAL7_CLK_DIV (0)

/* FOR EBI */
#define PERPHERAL8_CLK_DIV (1)

#define TIMERS_INTERRUPT_PERIORITY 4
#define TCON(x) T##x##CON 
#define TCONbits_TCKPS(x) T##x##CONbits.TCKPS
#define TCONbits_ON(x) T##x##CONbits.ON
#define IECbits_TIE(x, y) IEC##y##bits.T##x##IE
#define IFSbits_TIF(x, y) IFS##y##bits.T##x##IF
#define IPCbits_TIP(x, y) IPC##y##bits.T##x##IP
#define IPCbits_TIS(x, y) IPC##y##bits.T##x##IS
#define PR(x) PR##x

#define TIMER_1_ENABLE
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

enum
{
  TMR1_PRESCALE_1   = 0b00,
  TMR1_PRESCALE_8   = 0b01,
  TMR1_PRESCALE_64  = 0b10,
  TMR1_PRESCALE_256 = 0b11,
};

// timers from 2 to 9.

enum
{
  TMR_PRESCALE_1   = 0b000,
  TMR_PRESCALE_2   = 0b001,
  TMR_PRESCALE_4   = 0b010,
  TMR_PRESCALE_8   = 0b011,
  TMR_PRESCALE_16  = 0b100,
  TMR_PRESCALE_32  = 0b101,
  TMR_PRESCALE_64  = 0b110,
  TMR_PRESCALE_256 = 0b111,
};

uint32_t sysclk_timerfreq_get(void)
{
  return (SYS_FREQ / (PERPHERAL3_CLK_DIV + 1));
}

uint32_t sysclk_uartfreq_get(void)
{
  return (SYS_FREQ / (PERPHERAL2_CLK_DIV + 1));
}

#ifdef TIMER_1_ENABLE
void init_timer1(uint8_t freq, uint8_t prescale, uint8_t sub_periority)
{
  uint32_t tmr_clk = sysclk_timerfreq_get();

  TCON(1) = 0x0;
  TMR(1) = 0;
  TCONbits_TCKPS(1) = prescale;

  IECbits_TIE(1, 0) = 0;

  PR(1) = tmr_clk / freq / prescale;

  IFSbits_TIF(1, 0) = 0;
  IPCbits_TIP(1, 1) = TIMERS_INTERRUPT_PERIORITY;
  IPCbits_TIS(1, 1) = sub_periority;

  IECbits_TIE(1, 0) = 1;


  TCONbits_ON(1) = 1;
}
#endif

typedef struct 
{
  
}interrupt_t;

#ifdef TIMER_2_ENABLE
void init_timer2(uint8_t freq, uint8_t prescale, uint8_t sub_periority)
{
  uint32_t tmr_clk = sysclk_timerfreq_get();

  TCON(2) = 0x0;
  TMR(2) = 0;
  TCONbits_TCKPS(2) = prescale;

  IECbits_TIE(2, 0) = 0;

  PR(2) = tmr_clk / freq / prescale;

  IFSbits_TIF(2, 0) = 0;
  IPCbits_TIP(2, 2) = TIMERS_INTERRUPT_PERIORITY;
  IPCbits_TIS(2, 2) = sub_periority;

  IECbits_TIE(2, 0) = 1;


  TCONbits_ON(2) = 1;
}
#endif

#ifdef TIMER_3_ENABLE
void init_timer3(uint8_t freq, uint8_t prescale, uint8_t sub_periority)
{
  uint32_t tmr_clk = sysclk_timerfreq_get();

  TCON(3) = 0x0;
  TMR(3) = 0;
  TCONbits_TCKPS(3) = prescale;

  IECbits_TIE(3, 0) = 0;

  PR(3) = tmr_clk / freq / prescale;

  IFSbits_TIF(3, 0) = 0;
  IPCbits_TIP(3, 3) = TIMERS_INTERRUPT_PERIORITY;
  IPCbits_TIS(3, 3) = sub_periority;

  IECbits_TIE(3, 0) = 1;


  TCONbits_ON(3) = 1;
}
#endif

#ifdef TIMER_4_ENABLE
void init_timer4(uint8_t freq, uint8_t prescale, uint8_t sub_periority)
{
  uint32_t tmr_clk = sysclk_timerfreq_get();

  TCON(4) = 0x0;
  TMR(4) = 0;
  TCONbits_TCKPS(4) = prescale;

  IECbits_TIE(4, 0) = 0;

  PR(4) = tmr_clk / freq / prescale;

  IFSbits_TIF(4, 0) = 0;
  IPCbits_TIP(4, 4) = TIMERS_INTERRUPT_PERIORITY;
  IPCbits_TIS(4, 4) = sub_periority;

  IECbits_TIE(4, 0) = 1;


  TCONbits_ON(4) = 1;
}
#endif

#ifdef TIMER_5_ENABLE
void init_timer5(uint8_t freq, uint8_t prescale, uint8_t sub_periority)
{
  uint32_t tmr_clk = sysclk_timerfreq_get();

  TCON(5) = 0x0;
  TMR(5) = 0;
  TCONbits_TCKPS(5) = prescale;

  IECbits_TIE(5, 0) = 0;

  PR(5) = tmr_clk / freq / prescale;

  IFSbits_TIF(5, 0) = 0;
  IPCbits_TIP(5, 6) = TIMERS_INTERRUPT_PERIORITY;
  IPCbits_TIS(5, 6) = sub_periority;

  IECbits_TIE(5, 0) = 1;


  TCONbits_ON(5) = 1;
}
#endif

#ifdef TIMER_6_ENABLE
void init_timer6(uint8_t freq, uint8_t prescale, uint8_t sub_periority)
{
  uint32_t tmr_clk = sysclk_timerfreq_get();

  TCON(6) = 0x0;
  TMR(6) = 0;
  TCONbits_TCKPS(6) = prescale;

  IECbits_TIE(6, 0) = 0;

  PR(6) = tmr_clk / freq / prescale;

  IFSbits_TIF(6, 0) = 0;
  IPCbits_TIP(6, 7) = TIMERS_INTERRUPT_PERIORITY;
  IPCbits_TIS(6, 7) = sub_periority;

  IECbits_TIE(6, 0) = 1;


  TCONbits_ON(6) = 1;
}
#endif

#ifdef TIMER_7_ENABLE
void init_timer7(uint8_t freq, uint8_t prescale, uint8_t sub_periority)
{
  uint32_t tmr_clk = sysclk_timerfreq_get();

  TCON(7) = 0x0;
  TMR(7) = 0;
  TCONbits_TCKPS(7) = prescale;

  IECbits_TIE(7, 1) = 0;

  PR(7) = tmr_clk / freq / prescale;

  IFSbits_TIF(7, 1) = 0;
  IPCbits_TIP(7, 8) = TIMERS_INTERRUPT_PERIORITY;
  IPCbits_TIS(7, 8) = sub_periority;

  IECbits_TIE(7, 1) = 1;


  TCONbits_ON(7) = 1;
}
#endif

#ifdef TIMER_8_ENABLE
void init_timer8(uint8_t freq, uint8_t prescale, uint8_t sub_periority)
{
  uint32_t tmr_clk = sysclk_timerfreq_get();

  TCON(8) = 0x0;
  TMR(8) = 0;
  TCONbits_TCKPS(8) = prescale;

  IECbits_TIE(8, 1) = 0;

  PR(8) = tmr_clk / freq / prescale;

  IFSbits_TIF(8, 1) = 0;
  IPCbits_TIP(8, 9) = TIMERS_INTERRUPT_PERIORITY;
  IPCbits_TIS(8, 9) = sub_periority;

  IECbits_TIE(8, 1) = 1;


  TCONbits_ON(8) = 1;
}
#endif

#ifdef TIMER_9_ENABLE
void init_timer9(uint8_t freq, uint8_t prescale, uint8_t sub_periority)
{
  uint32_t tmr_clk = sysclk_timerfreq_get();

  TCON(9) = 0x0;
  TMR(9) = 0;
  TCONbits_TCKPS(9) = prescale;

  IECbits_TIE(9, 1) = 0;

  PR(9) = tmr_clk / freq / prescale;

  IFSbits_TIF(9, 1) = 0;
  IPCbits_TIP(9, 10) = TIMERS_INTERRUPT_PERIORITY;
  IPCbits_TIS(9, 10) = sub_periority;

  IECbits_TIE(9, 1) = 1;


  TCONbits_ON(9) = 1;
}
#endif

int main(int argc, char const *argv[])
{
  /* code */
  printf("This is sparta\n");

  return 0;
}


