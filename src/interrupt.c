#include <stdint.h>
#include <xc.h>
#include "pic32_config.h"

#define IFSbits_TIF(x, y)   IFS ## y ## bits.T ## x ## IF
#define IFSbits_URXIF(x, y) IFS ## y ## bits.U ## x ## RXIF

#define ipltmr1AUTO     iplAUTO(TIMER1_INT_PRIO)
#define ipltmr2AUTO     iplAUTO(TIMER2_INT_PRIO)
#define ipltmr3AUTO     iplAUTO(TIMER3_INT_PRIO)
#define ipltmr4AUTO     iplAUTO(TIMER4_INT_PRIO)
#define ipltmr5AUTO     iplAUTO(TIMER5_INT_PRIO)
#define ipltmr6AUTO     iplAUTO(TIMER6_INT_PRIO)
#define ipltmr7AUTO     iplAUTO(TIMER7_INT_PRIO)
#define ipltmr8AUTO     iplAUTO(TIMER8_INT_PRIO)
#define ipltmr9AUTO     iplAUTO(TIMER9_INT_PRIO)

#define ipluart1AUTO    iplAUTO(UART1_INT_PRIO)
#define ipluart2AUTO    iplAUTO(UART2_INT_PRIO)
#define ipluart3AUTO    iplAUTO(UART3_INT_PRIO)
#define ipluart4AUTO    iplAUTO(UART4_INT_PRIO)
#define ipluart5AUTO    iplAUTO(UART5_INT_PRIO)
#define ipluart6AUTO    iplAUTO(UART6_INT_PRIO)

#define iplAUTO(x)  ipl(x)
#define ipl(x)      ipl ## x ## AUTO

uint32_t global_tick = 0;

void __attribute__((vector(_TIMER_1_VECTOR), interrupt(ipltmr1AUTO),
                    nomips16)) _timer1_interrupt(void)
{
  IFS0bits.T1IF = 0;
  global_tick   += 1;
}

#if (TIMER_2_ENABLED == 1)
void __attribute__((weak)) timer_2_callback(void)
{
}

void __attribute__((vector(_TIMER_2_VECTOR), interrupt(ipltmr2AUTO),
                         nomips16)) _timer2_interrupt(void)
{
  IFSbits_TIF(2, 0) = 0;
  timer_2_callback();
}

#endif

#if (TIMER_3_ENABLED == 1)
void __attribute__((weak)) timer_3_callback(void)
{
}

void __attribute__((vector(_TIMER_3_VECTOR), interrupt(ipltmr3AUTO),
                         nomips16)) _timer3_interrupt(void)
{
  IFSbits_TIF(3, 0) = 0;
  timer_3_callback();
}

#endif

#if (TIMER_4_ENABLED == 1)
void __attribute__((weak)) timer_4_callback(void)
{
}

void __attribute__((vector(_TIMER_4_VECTOR), interrupt(ipltmr4AUTO),
                         nomips16)) _timer4_interrupt(void)
{
  IFSbits_TIF(4, 0) = 0;
  timer_4_callback();
}

#endif

#if (TIMER_5_ENABLED == 1)
void __attribute__((weak)) timer_5_callback(void)
{
}

void __attribute__((vector(_TIMER_5_VECTOR), interrupt(ipltmr5AUTO),
                         nomips16)) _timer5_interrupt(void)
{
  IFSbits_TIF(5, 0) = 0;
  timer_5_callback();
}

#endif

#if (TIMER_6_ENABLED == 1)
void __attribute__((weak)) timer_6_callback(void)
{
}

void __attribute__((vector(_TIMER_6_VECTOR), interrupt(ipltmr6AUTO),
                         nomips16)) _timer6_interrupt(void)
{
  IFSbits_TIF(6, 0) = 0;
  timer_6_callback();
}

#endif

#if (TIMER_7_ENABLED == 1)
void __attribute__((weak)) timer_7_callback(void)
{
}

void __attribute__((vector(_TIMER_7_VECTOR), interrupt(ipltmr7AUTO),
                         nomips16)) _timer7_interrupt(void)
{
  IFSbits_TIF(7, 1) = 0;
  timer_7_callback();
}

#endif

#if (TIMER_8_ENABLED == 1)
void __attribute__((weak)) timer_8_callback(void)
{
}

void __attribute__((vector(_TIMER_8_VECTOR), interrupt(ipltmr8AUTO),
                         nomips16)) _timer8_interrupt(void)
{
  IFSbits_TIF(8, 1) = 0;
  timer_8_callback();
}

#endif

#if (TIMER_9_ENABLED == 1)
void __attribute__((weak)) timer_9_callback(void)
{
}

void __attribute__((vector(_TIMER_9_VECTOR), interrupt(ipltmr9AUTO),
                         nomips16)) _timer9_interrupt(void)
{
  IFSbits_TIF(9, 1) = 0;
  timer_9_callback();
}

#endif

#if (PIC32_UART_1_ENABLED == 1)
extern void uartrx_1_callback(void);
void __attribute__((vector(_UART1_RX_VECTOR), interrupt(ipluart1AUTO),
                         nomips16)) _uartrx1_interrupt(void)
{
  uartrx_1_callback();
  IFSbits_URXIF(1, 3) = 0;
}
#endif

#if (PIC32_UART_2_ENABLED == 1)
extern void uartrx_2_callback(void);
void __attribute__((vector(_UART2_RX_VECTOR), interrupt(ipluart2AUTO),
                         nomips16)) _uartrx2_interrupt(void)
{
  uartrx_2_callback();
  IFSbits_URXIF(2, 4) = 0;
}
#endif

#if (PIC32_UART_3_ENABLED == 1)
extern void uartrx_3_callback(void);
void __attribute__((vector(_UART3_RX_VECTOR), interrupt(ipluart3AUTO),
                         nomips16)) _uartrx3_interrupt(void)
{
  uartrx_3_callback();
  IFSbits_URXIF(3, 4) = 0;
}
#endif

#if (PIC32_UART_4_ENABLED == 1)
extern void uartrx_4_callback(void);
void __attribute__((vector(_UART4_RX_VECTOR), interrupt(ipluart4AUTO),
                         nomips16)) _uartrx4_interrupt(void)
{
  uartrx_4_callback();
  IFSbits_URXIF(4, 5) = 0;
}
#endif

#if (PIC32_UART_5_ENABLED == 1)
extern void uartrx_5_callback(void);
void __attribute__((vector(_UART5_RX_VECTOR), interrupt(ipluart5AUTO),
                         nomips16)) _uartrx5_interrupt(void)
{
  uartrx_5_callback();
  IFSbits_URXIF(5, 5) = 0;
}
#endif

#if (PIC32_UART_6_ENABLED == 1)
extern void uartrx_6_callback(void);
void __attribute__((vector(_UART6_RX_VECTOR), interrupt(ipluart6AUTO),
                         nomips16)) _uartrx6_interrupt(void)
{
  uartrx_6_callback();
  IFSbits_URXIF(6, 5) = 0;
}
#endif

void interrupt_init(void)
{
  /* enable interrupts */

  INTCONbits.MVEC = 1; /* this enables the multi Vectored interrupt to tell the
                        * microprocessor */

  /* to allow different handlers for each different type of interrupt (timer2,
   * timer 3) */

  asm volatile ("ei");
}

uint32_t interrupt_tick_get(void)
{
  return global_tick;
}