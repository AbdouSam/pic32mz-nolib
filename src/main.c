/*
 * @Author AbdouSam
 * @brief, simplest code to test-run a PIC32MZ without any use of libraries
 *
 */
#include <xc.h>
#include "uart.h"
#include "sysclk.h"
#include "gpio.h"
#include "uart.h"
#include "delay.h"

// This define is to set the  µC to run on internal clock

//#define CONFIG_CPU_USE_FRC

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
#pragma config CP =         OFF // code protect

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
#pragma config FWDTEN =     OFF // WatchDog timer
#pragma config WINDIS =     NORMAL
#pragma config FWDTWINSZ =  WINSZ_25
#pragma config DMTCNT =     DMT31
#pragma config FDMTEN =     OFF // Deadman timer

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

static unsigned int volatile global_tick = 0;

void print_str4(char *s)
{
  while ( *s != '\0')
  {
    uart_tx_char(PIC32_UART_4, *s++);
  }
}

void UART4_init(int baud_rate)
{
  // Set rs485 bit to output
  gpio_state_set(PIC32_PORTA, 10, true);
  
  init_uart(PIC32_UART_4, NO_PARITY_8_BIT_DATA, ONE_STOP_BIT, baud_rate);

  print_str4("Hello UART \n");
}


char read_char4(void)
{
  if (uart_rx_any(PIC32_UART_4)) // Data ready
  {
    return uart_rx_char(PIC32_UART_4);
  }
  return 0;
}

void init_timer1(int freq)
{
  T1CON = 0x0;
  TMR1 = 0;

  IEC0bits.T1IE = 0;

  PR1 = SYS_FREQ / 5 / freq;

  IFS0bits.T1IF = 0;
  IPC1bits.T1IP = 3;
  IPC1bits.T1IS = 1;
  IEC0bits.T1IE = 1;

  T1CONbits.TON = 1;
}

//the next routine or the call for the interrupt tells the compiler to set
// ipl3soft, interrupt periority level 3, to use a software context swtiching, 
// or manually save all 32 GPR or only the necessary ones, into the stack and
// retreive them once we return from the interrupt
// other options may use the shadow registers, MIPS32 M4k core has a set of shadow registers
// for each level of periority these can be set, in the PIC32MZ with the PRISS 
// PRIORITY SHADOW SELECT REGISTER, note that the MVEC bit in INTCON should be active

void __attribute__((vector(_TIMER_1_VECTOR), interrupt(ipl3AUTO), nomips16)) _timer1_interrupt(void)
{
  IFS0bits.T1IF = 0;
  global_tick+=1;
}

int main(void)
{

  sysclk_init();

  gpio_init();
  /*
  ANSELB = 0; // PIC starts all pins as analog, needs to be deactivated to be used
  TRISBCLR = 1 << 13; //set bit 13 port B to output
  TRISBCLR = 1 << 12; //set bit 12 port B to output
  */

  gpio_state_set(PIC32_PORTB ,12, true);
  gpio_state_set(PIC32_PORTB ,13, true);
  // For the interrupt

  INTCONbits.MVEC = 1; // this enables the multi Vectored interrupt to tell the microprocessor
  // to allow different handlers for each different type of interrupt (timer2, timer 3)
  init_timer1(1000);

  delay_ms(1000);

  UART4_init(115200);

  unsigned int millis = global_tick;

  for (;; )
    {

      if (global_tick - millis >= 1000)
      {
        gpio_state_toggle(PIC32_PORTB ,12);
        gpio_state_toggle(PIC32_PORTB ,13);

        millis = global_tick;
      }
    }

  return 0;
}