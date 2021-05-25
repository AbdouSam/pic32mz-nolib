/*
 * @Author AbdouSam
 * @brief, simplest code to test-run a PIC32MZ without any use of libraries
 *
 */
#include <stdbool.h>

#include <xc.h>
#include <string.h>

#include "pic32_config.h"
#include "debug.h"

#include "helpers.h"
#include "sysclk.h"
#include "gpio.h"
#include "uart.h"
#include "delay.h"
#include "timer.h"
#include "interrupt.h"
#include "i2c.h"
#include "rtc.h"
#include "app.h"
#include "uart.h"
#include "plc.h"
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


static char          c_tmp;
static char          rx_tmp[10];
static int           rx_index = 0;
static unsigned char rtc_val[7];
static unsigned int  millis = 0;
static bool          wdt_clear_flag = true;


static bool first_start = true;
static bool relay_started = false;
static uint32_t relay_time_now = 0;
static uint32_t plc_run_timing = 0;
static bool plc_run_flag = false;

static void wdt_clear(void)
{
  volatile uint16_t * wdtclrkey;
  asm volatile("di");
  /* get address of upper 16-bit word of the WDTCON SFR */
  wdtclrkey     = ( (volatile uint16_t *)&WDTCON ) + 1;
  *wdtclrkey    = 0x5743;
  asm volatile("ei");
}

static char read_char(void)
{
  if (uart_rx_any(PIC32_UART_4)) /* Data ready */
    {
      return uart_rx_char(PIC32_UART_4);
    }
  return 0;
}

/**
 * override weak def in interrupt.c 
 */
void timer_2_callback(void)
{
  gpio_state_toggle(LED_RED);
  gpio_state_toggle(LED_GREEN);
  wdt_clear_flag = true;
}

void timer_4_callback(void)
{
  plc_run_flag = true;
  plc_updatetime();
}

void uart_callback(void)
{

  gpio_state_toggle(pinE9);
  c_tmp = read_char();
  if (c_tmp != '\n')
    rx_tmp[rx_index] = c_tmp;

  rx_index++;
  if (rx_index > 9)
  {
    rx_index = 0;
  }
}

int app_init(void)
{

  sysclk_init();

  /* Initial IO as it is set in pic32_config.h */
  gpio_init();

  gpio_state_clear(LED_ORANGE);
  gpio_state_clear(LED_RED);
  gpio_state_clear(LED_GREEN);

  uart_rxi_set(PIC32_UART_4, 3, IF_RBUF_NOT_EMPTY, uart_callback);
  
  debug_init();

  delay_ms(100);

  debug_print("Hello World\n");

  timer_1_init();

  if (timer_init(PIC32_TIMER_2, 2 /* Hz */, 0) < 0)
  {
    debug_print("Timer 2 failed to init\n");
  } 

  /* Timer for PLC tick */
  if (timer_init(PIC32_TIMER_4, 1000 /* Hz */, 0) < 0)
  {
    debug_print("Timer 4 failed to init\n");
  } 

  i2c_init(100000);

  rtc_init();

  plc_init();

  interrupt_init();

  return 0;
}

void app_task(void)
{
  int c_local;

  if (interrupt_tick_get() - millis >= 1000)
    {
      /* test timer/interrupt/gpio */
      gpio_state_toggle(LED_ORANGE);

      millis = interrupt_tick_get();
      get_current_time();
      /* test read rtc/ i2c */
      rtc_read_time(rtc_val);
      debug_print("Run time plc : %d ns\n", plc_run_timing * 10);
    }


  if (plc_run_flag)
  {
    plc_run_flag = false;
    plc_run();
  }

  /* test read the uart */

  if (gpio_state_get(DIG_IN4))
    {
      plc_set_mem0(20.0);
    }
  else if (gpio_state_get(DIG_IN8))
    {
      plc_set_mem0(1.0);
    }

  if (gpio_state_get(RELAY_OUT5))
    {
      if (first_start == true)
      {
        first_start = false;
        relay_time_now = interrupt_tick_get();
        relay_started = true;
      }
    }
  else
    {
      if (relay_started)
      {
        relay_started = false;
        first_start = true;
        debug_print("Time on %d ms\n", interrupt_tick_get() - relay_time_now);
      }
    }

  if ((c_tmp >= 'a') && (c_tmp <= 'z'))
    {
      uart_tx_char(PIC32_UART_4, c_tmp);
      uart_tx_char(PIC32_UART_4, '\n');
      c_tmp = 0;
    }

  if (c_tmp == '\n')
    {
      debug_print("Word: ");

      for (c_local = 0; c_local<10; c_local++)
        debug_putc(rx_tmp[c_local]);

      debug_putc('\n');
      c_tmp = 0;
    }

  if (wdt_clear_flag)
  {
    wdt_clear_flag = false;
    wdt_clear();
  }

}