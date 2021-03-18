#include <stdint.h>
#include <xc.h>
#include "pic32_config.h"
#include<sys/attribs.h>
#include<gpio.h>
#define IFSbits_TIF(x, y)  IFS ## y ## bits.T ## x ## IF

#define ipltmr1AUTO     iplAUTO(TIMER1_INTERRUPT_PERIORITY)
#define ipltmr2AUTO     iplAUTO(TIMER2_INTERRUPT_PERIORITY)
#define ipltmr3AUTO     iplAUTO(TIMER3_INTERRUPT_PERIORITY)
#define ipltmr4AUTO     iplAUTO(TIMER4_INTERRUPT_PERIORITY)
#define ipltmr5AUTO     iplAUTO(TIMER5_INTERRUPT_PERIORITY)
#define ipltmr6AUTO     iplAUTO(TIMER6_INTERRUPT_PERIORITY)
#define ipltmr7AUTO     iplAUTO(TIMER7_INTERRUPT_PERIORITY)
#define ipltmr8AUTO     iplAUTO(TIMER8_INTERRUPT_PERIORITY)
#define ipltmr9AUTO     iplAUTO(TIMER9_INTERRUPT_PERIORITY)
//TODO define GPIO interrupt priority
#define GPIO_INTERRUPT_PRIORITY 2
//#define ipl2AUTO       iplAUTO(GPIO_INTERRUPT_PRIORITY)
#define iplAUTO(x)  ipl(x)
#define ipl(x)      ipl ## x ## AUTO

#define TABLE_ENTRIES 3

uint32_t global_tick = 0;

volatile unsigned int oldA = 0, newA = 0, newB=0, oldB=0,
         newC = 0, oldC = 0, newD = 0, oldD = 0,
         newE = 0, oldE = 0, newF = 0, oldF = 0, 
         newG = 0, oldG = 0, newH = 0, oldH = 0, 
         newJ = 0, oldJ = 0, newK = 0, oldK = 0;         


//Interrupt service routines for GPIO interrupts
//User defined callback function
void __attribute((weak)) gpio_A_callback(void)
{
}

void __ISR(_CHANGE_NOTICE_A_VECTOR, ipl2AUTO) CNISRA(void){
    newA = PORTA;    //Save current port state
    gpio_A_callback();
    oldA = newA;
    CNFA = 0;             //Clear flag register
    IFS3bits.CNAIF = 0;   //clear interrupt flag bit 
}

void __attribute__((weak)) gpio_B_callback(void)
{
}

void __ISR(_CHANGE_NOTICE_B_VECTOR, ipl2AUTO) CNISRB(void){
    newB = PORTB;                                         
    oldB = newB;
    gpio_B_callback();
    CNFB = 0;
    IFS3bits.CNBIF = 0;                                     
}



void __attribute__((weak)) gpio_C_callback(void)
{
}

void __ISR(_CHANGE_NOTICE_C_VECTOR, ipl2AUTO) CNISRC(void){
    newC = PORTC;                                         
    oldC = newC;
    gpio_C_callback();
    CNFC = 0;
    IFS3bits.CNCIF = 0;                                     
}
void __attribute__((weak)) gpio_D_callback(void)
{
}
void __ISR(_CHANGE_NOTICE_D_VECTOR, ipl2AUTO) CNISRD(void){
    newD = PORTD;                                         
    oldD = newD;
    gpio_D_callback();
    CNFD = 0;
    IFS3bits.CNDIF = 0;                                     
}


void __attribute__((weak)) gpio_E_callback(void)
{
}
void __ISR(_CHANGE_NOTICE_E_VECTOR, ipl2AUTO) CNISRE(void){
    newE = PORTE;                                         
    oldE = newE;
    gpio_E_callback();
    CNFE = 0;
    IFS3bits.CNEIF = 0;                                     
}

void __attribute__((weak)) gpio_F_callback(void)
{
}

void __ISR(_CHANGE_NOTICE_F_VECTOR, ipl2AUTO) CNISRF(void){
    newF = PORTF;                                         
    oldF = newF;
    gpio_F_callback();
    CNFF = 0;
    IFS3bits.CNFIF = 0;                                     
}

void __attribute__((weak)) gpio_G_callback(void)
{
}
void __ISR(_CHANGE_NOTICE_G_VECTOR, ipl2AUTO) CNISRG(void){
    newG = PORTG;                                         
    oldG = newG;
    gpio_G_callback();
    CNFG = 0;
    IFS3bits.CNGIF = 0;                                     
}

void __attribute__((weak)) gpio_H_callback(void)
{
}

void __ISR(_CHANGE_NOTICE_H_VECTOR, ipl2AUTO) CNISRH(void){
    newH = PORTH;                                         
    oldH = newH;
    gpio_H_callback();
    CNFH = 0;
    IFS3bits.CNHIF = 0;                                     
}

void __attribute__((weak)) gpio_J_callback(void)
{
}

void __ISR(_CHANGE_NOTICE_J_VECTOR, ipl2AUTO) CNISRJ(void){
    newJ = PORTJ;                                         
    oldJ = newJ;
    gpio_J_callback();
    CNFJ = 0;
    IFS3bits.CNJIF = 0;                                     
}

void __attribute__((weak)) gpio_K_callback(void)
{
}
void __ISR(_CHANGE_NOTICE_K_VECTOR, ipl2AUTO) CNISRK(void){
    newK = PORTK;                                         
    oldK = newK;
    gpio_K_callback();
    CNFK = 0;
    IFS3bits.CNKIF = 0;                                     
}

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
