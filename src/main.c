/*
 * @Author AbdouSam
 * @brief, simplest code to test-run a PIC32MZ without any use of libraries
 *
 */
#include <stdbool.h>

#include <xc.h>
#include "sysclk.h"
#include"gpio.h"
#include"delay.h"
#include"timer.h"
#include"interrupt.h"
#include"pic32_config.h"

/* This define is to set the  µC to run on internal clock
 * config is set to run CPU at 200 Mhz,
 * with internal or 24Mhz external clock
 */

/*#define CONFIG_CPU_USE_FRC */

/*** DEVCFG0 ***/
#pragma config DEBUG =      OFF
#pragma config JTAGEN =     OFF
#pragma config ICESEL =     ICS_PGx1
#pragma config TRCEN =      OFF
#pragma config BOOTISA =    MIPS32
#pragma config FECCCON =    OFF_UNLOCKED
#pragma config FSLEEP =     OFF
#pragma config DBGPER =     PG_ALL
#pragma config SMCLR =      MCLR_NORM
#pragma config SOSCGAIN =   GAIN_2X
#pragma config SOSCBOOST =  ON
#pragma config POSCGAIN =   GAIN_0_5X
#pragma config POSCBOOST =  ON
#pragma config EJTAGBEN =   NORMAL
#pragma config CP =         OFF /* code protect */

/*** DEVCFG1 ***/

#pragma config FNOSC =      SPLL
#pragma config DMTINTV =    WIN_127_128
#pragma config FSOSCEN =    OFF
#pragma config IESO =       OFF

#ifdef CONFIG_CPU_USE_FRC
#pragma config POSCMOD =    OFF
#else
#pragma config POSCMOD =    HS
#endif

#pragma config OSCIOFNC =   OFF
#pragma config FCKSM =      CSECME
#pragma config WDTPS =      PS1048576
#pragma config WDTSPGM =    STOP
#pragma config FWDTEN =     OFF /* WatchDog timer */
#pragma config WINDIS =     NORMAL
#pragma config FWDTWINSZ =  WINSZ_25
#pragma config DMTCNT =     DMT31
#pragma config FDMTEN =     OFF /* Deadman timer */

/*** DEVCFG2 ***/

#ifdef CONFIG_CPU_USE_FRC
#pragma config FPLLIDIV =   DIV_1
#else
#pragma config FPLLIDIV =   DIV_3
#endif

#pragma config FPLLRNG =    RANGE_5_10_MHZ

#ifdef CONFIG_CPU_USE_FRC
#pragma config FPLLICLK =   PLL_FRC
#else
#pragma config FPLLICLK =   PLL_POSC
#endif

#pragma config FPLLMULT =   MUL_50
#pragma config FPLLODIV =   DIV_2
#pragma config UPLLFSEL =   FREQ_24MHZ

/*** DEVCFG3 ***/

#pragma config USERID =     0xffff
#pragma config FMIIEN =     OFF
#pragma config FETHIO =     ON
#pragma config PGL1WAY =    ON
#pragma config PMDL1WAY =   ON
#pragma config IOL1WAY =    ON
#pragma config FUSBIDIO =   OFF

/*** BF1SEQ0 ***/

#pragma config TSEQ =       0x0000
#pragma config CSEQ =       0xffff
//void timer_6_callback(void){
 //gpio_state_toggle(pinB13);

//};
/*void io0interrupt_callback(void){
    gpio_state_toggle(pinE9);
    delay_ms(1000);
    };*/

int main(void){
/* init */
 sysclk_init();
/*set IO*/
 gpio_output_set(pinE9);
 gpio_output_set(pinB12);
/*configure interrupts */
 //CNCONAbits.ON = 1;
 //CNENAbits.CNIEA14 = 1;
gpio_input_set(pinA14);
gpio_set_interrupt(pinA14);
// init_timer6(10, TMR_PRESCALE_256, 0);
 interrupt_init();
/* infinite loop */
 while(true){
     gpio_state_toggle(pinB12);
     delay_ms(1000);
 }
    return 0;
 }
;
  
