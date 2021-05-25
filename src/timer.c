#include <stdint.h>
#include <stdio.h>
#include <xc.h>

#include "pic32_config.h"
#include "sysclk.h"
#include "interrupt.h"
#include "helpers.h"
#include "timer.h"

#define TMR_TCKPS_BIT_1 (4)
#define TMR_TCKPS_BIT_2 (5)
#define TMR_TCKPS_BIT_3 (6)
#define TMR_ON_BIT      (15)

#define TMR1_TCKPS_MASK ((1 << TMR_TCKPS_BIT_1) | (1 << TMR_TCKPS_BIT_2))

#define TMR_TCKPS_MASK  ((1 << TMR_TCKPS_BIT_1) | \
                        (1 << TMR_TCKPS_BIT_2) | \
                        (1 << TMR_TCKPS_BIT_3))

#define TCON(x)             T ## x ## CON
#define TCONbits_TCKPS(x)   T ## x ## CONbits.TCKPS
#define TCONbits_ON(x)      T ## x ## CONbits.ON
#define IECbits_TIE(x, y)   IEC ## y ## bits.T ## x ## IE
#define IFSbits_TIF(x, y)   IFS ## y ## bits.T ## x ## IF
#define IPCbits_TIP(x, y)   IPC ## y ## bits.T ## x ## IP
#define IPCbits_TIS(x, y)   IPC ## y ## bits.T ## x ## IS
#define PR(x)               PR ## x
#define TMR(x)              TMR ## x

#define MAX_TMR_PR_VALUE (65535U)

static uint16_t tmr1_prescale_value[] = {
  1, 8, 64, 256
};
static uint16_t tmrs_prescale_value[] = {
  1, 2, 4, 8, 16, 32, 64, 256
};

static uint32_t volatile * const tmr_con[PIC32_TIMER_MAX] =
{
  &T1CON,
  &T2CON,
  &T3CON,
  &T4CON,
  &T5CON,
  &T6CON,
  &T7CON,
  &T8CON,
  &T9CON,
};

static uint32_t volatile * const tmr_loadval[PIC32_TIMER_MAX] =
{
  &PR1,
  &PR2,
  &PR3,
  &PR4,
  &PR5,
  &PR6,
  &PR7,
  &PR8,
  &PR9,
};

static uint32_t volatile * const tmr_tmr[PIC32_TIMER_MAX] =
{
  &TMR1,
  &TMR2,
  &TMR3,
  &TMR4,
  &TMR5,
  &TMR6,
  &TMR7,
  &TMR8,
  &TMR9,
};

static intr_regs_t const timer_intr[PIC32_TIMER_MAX] =
{
  {
    .flag      = &IFS0,
    .enable    = &IEC0,
    .prio      = &IPC1,
    .prioshift = IPC_PRIO_SHIFT_0,
    .subpshift = IPC_SUBP_SHIFT_0,
    .defprio   = TIMER1_INT_PRIO,
    .ibit      = 4,
  },
  {
    .flag      = &IFS0,
    .enable    = &IEC0,
    .prio      = &IPC2,
    .prioshift = IPC_PRIO_SHIFT_8,
    .subpshift = IPC_SUBP_SHIFT_8,
    .defprio   = TIMER2_INT_PRIO,
    .ibit      = 9,
  },
  {
    .flag      = &IFS0,
    .enable    = &IEC0,
    .prio      = &IPC3,
    .prioshift = IPC_PRIO_SHIFT_16,
    .subpshift = IPC_SUBP_SHIFT_16,
    .defprio   = TIMER3_INT_PRIO,
    .ibit      = 14,
  },
  {
    .flag      = &IFS0,
    .enable    = &IEC0,
    .prio      = &IPC4,
    .prioshift = IPC_PRIO_SHIFT_24,
    .subpshift = IPC_SUBP_SHIFT_24,
    .defprio   = TIMER4_INT_PRIO,
    .ibit      = 19,
  },
  {
    .flag      = &IFS0,
    .enable    = &IEC0,
    .prio      = &IPC6,
    .prioshift = IPC_PRIO_SHIFT_0,
    .subpshift = IPC_SUBP_SHIFT_0,
    .defprio   = TIMER5_INT_PRIO,
    .ibit      = 24,
  },
  {
    .flag      = &IFS0,
    .enable    = &IEC0,
    .prio      = &IPC7,
    .prioshift = IPC_PRIO_SHIFT_0,
    .subpshift = IPC_SUBP_SHIFT_0,
    .defprio   = TIMER6_INT_PRIO,
    .ibit      = 28,
  },
  {
    .flag      = &IFS1,
    .enable    = &IEC1,
    .prio      = &IPC8,
    .prioshift = IPC_PRIO_SHIFT_0,
    .subpshift = IPC_SUBP_SHIFT_0,
    .defprio   = TIMER7_INT_PRIO,
    .ibit      = 0,
  },
  {
    .flag      = &IFS1,
    .enable    = &IEC1,
    .prio      = &IPC9,
    .prioshift = IPC_PRIO_SHIFT_0,
    .subpshift = IPC_SUBP_SHIFT_0,
    .defprio   = TIMER8_INT_PRIO,
    .ibit      = 4,
  },
  {
    .flag      = &IFS1,
    .enable    = &IEC1,
    .prio      = &IPC10,
    .prioshift = IPC_PRIO_SHIFT_0,
    .subpshift = IPC_SUBP_SHIFT_0,
    .defprio   = TIMER9_INT_PRIO,
    .ibit      = 8,
  },

};


void timer_1_init(void)
{
  uint32_t tmr_clk = sysclk_timerfreq_get();

  *tmr_con[PIC32_TIMER_1] = 0x0;
  *tmr_tmr[PIC32_TIMER_1] = 0x0;
  
  *tmr_loadval[PIC32_TIMER_1] = tmr_clk / TIMER_1_FREQ / tmr1_prescale_value[TMR_PRESCALE_1];
  BIT_MASK(tmr_con[PIC32_TIMER_1], TMR1_TCKPS_MASK, (TMR_PRESCALE_1 << TMR_TCKPS_BIT_1));

  BIT_CLR(timer_intr[PIC32_TIMER_1].enable, timer_intr[PIC32_TIMER_1].ibit);
  
  BIT_CLR(timer_intr[PIC32_TIMER_1].flag, timer_intr[PIC32_TIMER_1].ibit);

  BIT_WRITE(timer_intr[PIC32_TIMER_1].prio, INT_PRIO_BITLEN, 
            timer_intr[PIC32_TIMER_1].prioshift,
            timer_intr[PIC32_TIMER_1].defprio);

  BIT_WRITE(timer_intr[PIC32_TIMER_1].prio, INT_SUBP_BITLEN,
            timer_intr[PIC32_TIMER_1].subpshift,
            0);

  BIT_SET(timer_intr[PIC32_TIMER_1].enable, timer_intr[PIC32_TIMER_1].ibit);

  BIT_SET(tmr_con[PIC32_TIMER_1], TMR_ON_BIT);

}

int timer_init(pic32_timer_t timer_id, uint32_t freq, uint8_t subp)
{
  uint32_t tmr_clk;
  uint32_t tmr_preload;
  int prs_idx;

  if (timer_id < PIC32_TIMER_2 || timer_id > PIC32_TIMER_9)
  {
    return -1;
  }

  *tmr_con[timer_id] = 0x0;
  *tmr_tmr[timer_id] = 0x0;

  tmr_clk = sysclk_timerfreq_get();

  /* check if a prescaler is required */
  for (prs_idx = 0; prs_idx < TMR_PRESCALE_MAX_INDX; prs_idx++)
  {
    tmr_preload = tmr_clk / freq / tmrs_prescale_value[prs_idx];

    if (tmr_preload <= MAX_TMR_PR_VALUE && tmr_preload > 0)
      break;
  }

  if (prs_idx == TMR_PRESCALE_MAX_INDX)
  {
    /* preload value failed */
    return -1;
  }

  *tmr_loadval[timer_id] = tmr_clk / freq / tmrs_prescale_value[prs_idx];

  BIT_MASK(tmr_con[timer_id], TMR_TCKPS_MASK, (prs_idx << TMR_TCKPS_BIT_1));


  BIT_CLR(timer_intr[timer_id].enable, timer_intr[timer_id].ibit);
  
  BIT_CLR(timer_intr[timer_id].flag, timer_intr[timer_id].ibit);

  BIT_WRITE(timer_intr[timer_id].prio, INT_PRIO_BITLEN, 
            timer_intr[timer_id].prioshift,
            timer_intr[timer_id].defprio);

  BIT_WRITE(timer_intr[timer_id].prio, INT_SUBP_BITLEN,
            timer_intr[timer_id].subpshift,
            subp);

  BIT_SET(timer_intr[timer_id].enable, timer_intr[timer_id].ibit);

  BIT_SET(tmr_con[timer_id], TMR_ON_BIT);

  return 0;
}

int timer_stop(pic32_timer_t timer_id)
{
   if (timer_id < PIC32_TIMER_2 || timer_id > PIC32_TIMER_9)
  {
    return -1;
  }


  BIT_CLR(tmr_con[timer_id], TMR_ON_BIT);

  return 0;
}

#if (TIMER_2_ENABLED == 1)
void init_timer2(uint32_t freq, timers_prescaler_t prescaler,
                 uint8_t sub_periority)
{
  uint32_t tmr_clk = sysclk_timerfreq_get();

  TCON(2) = 0x0;
  TMR(2) = 0;
  TCONbits_TCKPS(2) = prescaler;

  IECbits_TIE(2, 0) = 0;

  PR(2) = tmr_clk / freq / tmrs_prescale_value[prescaler];

  IFSbits_TIF(2, 0) = 0;
  IPCbits_TIP(2, 2) = TIMER2_INT_PRIO;
  IPCbits_TIS(2, 2) = sub_periority;

  IECbits_TIE(2, 0) = 1;

  TCONbits_ON(2) = 1;
}

#endif

#if (TIMER_3_ENABLED == 1)
void init_timer3(uint32_t freq, timers_prescaler_t prescaler,
                 uint8_t sub_periority)
{
  uint32_t tmr_clk = sysclk_timerfreq_get();

  TCON(3) = 0x0;
  TMR(3) = 0;
  TCONbits_TCKPS(3) = prescaler;

  IECbits_TIE(3, 0) = 0;

  PR(3) = tmr_clk / freq / tmrs_prescale_value[prescaler];

  IFSbits_TIF(3, 0) = 0;
  IPCbits_TIP(3, 3) = TIMER3_INT_PRIO;
  IPCbits_TIS(3, 3) = sub_periority;

  IECbits_TIE(3, 0) = 1;

  TCONbits_ON(3) = 1;
}

#endif

#if (TIMER_4_ENABLED == 1)
void init_timer4(uint32_t freq, timers_prescaler_t prescaler,
                 uint8_t sub_periority)
{
  uint32_t tmr_clk = sysclk_timerfreq_get();

  TCON(4) = 0x0;
  TMR(4) = 0;
  TCONbits_TCKPS(4) = prescaler;

  IECbits_TIE(4, 0) = 0;

  PR(4) = tmr_clk / freq / tmrs_prescale_value[prescaler];

  IFSbits_TIF(4, 0) = 0;
  IPCbits_TIP(4, 4) = TIMER4_INT_PRIO;
  IPCbits_TIS(4, 4) = sub_periority;

  IECbits_TIE(4, 0) = 1;

  TCONbits_ON(4) = 1;
}

#endif

#if (TIMER_5_ENABLED == 1)
void init_timer5(uint32_t freq, timers_prescaler_t prescaler,
                 uint8_t sub_periority)
{
  uint32_t tmr_clk = sysclk_timerfreq_get();

  TCON(5) = 0x0;
  TMR(5) = 0;
  TCONbits_TCKPS(5) = prescaler;

  IECbits_TIE(5, 0) = 0;

  PR(5) = tmr_clk / freq / tmrs_prescale_value[prescaler];

  IFSbits_TIF(5, 0) = 0;
  IPCbits_TIP(5, 6) = TIMER5_INT_PRIO;
  IPCbits_TIS(5, 6) = sub_periority;

  IECbits_TIE(5, 0) = 1;

  TCONbits_ON(5) = 1;
}

#endif

#if (TIMER_6_ENABLED == 1)
void init_timer6(uint32_t freq, timers_prescaler_t prescaler,
                 uint8_t sub_periority)
{
  uint32_t tmr_clk = sysclk_timerfreq_get();

  TCON(6) = 0x0;
  TMR(6) = 0;
  TCONbits_TCKPS(6) = prescaler;

  IECbits_TIE(6, 0) = 0;

  PR(6) = tmr_clk / freq / tmrs_prescale_value[prescaler];

  IFSbits_TIF(6, 0) = 0;
  IPCbits_TIP(6, 7) = TIMER6_INT_PRIO;
  IPCbits_TIS(6, 7) = sub_periority;

  IECbits_TIE(6, 0) = 1;

  TCONbits_ON(6) = 1;
}

#endif

#if (TIMER_7_ENABLED == 1)
void init_timer7(uint32_t freq, timers_prescaler_t prescaler,
                 uint8_t sub_periority)
{
  uint32_t tmr_clk = sysclk_timerfreq_get();

  TCON(7) = 0x0;
  TMR(7) = 0;
  TCONbits_TCKPS(7) = prescaler;

  IECbits_TIE(7, 1) = 0;

  PR(7) = tmr_clk / freq / tmrs_prescale_value[prescaler];

  IFSbits_TIF(7, 1) = 0;
  IPCbits_TIP(7, 8) = TIMER7_INT_PRIO;
  IPCbits_TIS(7, 8) = sub_periority;

  IECbits_TIE(7, 1) = 1;

  TCONbits_ON(7) = 1;
}

#endif

#if (TIMER_8_ENABLED == 1)
void init_timer8(uint32_t freq, timers_prescaler_t prescaler,
                 uint8_t sub_periority)
{
  uint32_t tmr_clk = sysclk_timerfreq_get();

  TCON(8) = 0x0;
  TMR(8) = 0;
  TCONbits_TCKPS(8) = prescaler;

  IECbits_TIE(8, 1) = 0;

  PR(8) = tmr_clk / freq / tmrs_prescale_value[prescaler];

  IFSbits_TIF(8, 1) = 0;
  IPCbits_TIP(8, 9) = TIMER8_INT_PRIO;
  IPCbits_TIS(8, 9) = sub_periority;

  IECbits_TIE(8, 1) = 1;

  TCONbits_ON(8) = 1;
}

#endif

#if (TIMER_9_ENABLED == 1)
void init_timer9(uint32_t freq, timers_prescaler_t prescaler,
                 uint8_t sub_periority)
{
  uint32_t tmr_clk = sysclk_timerfreq_get();

  TCON(9) = 0x0;
  TMR(9) = 0;
  TCONbits_TCKPS(9) = prescaler;

  IECbits_TIE(9, 1) = 0;

  PR(9) = tmr_clk / freq / tmrs_prescale_value[prescaler];

  IFSbits_TIF(9, 1) = 0;
  IPCbits_TIP(9, 10)    = TIMER9_INT_PRIO;
  IPCbits_TIS(9, 10)    = sub_periority;

  IECbits_TIE(9, 1) = 1;

  TCONbits_ON(9) = 1;
}

#endif