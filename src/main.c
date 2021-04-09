/*
 * @Author AbdouSam
 * @brief, simplest code to test-run a PIC32MZ without any use of libraries
 *
 */
#include <stdbool.h>

#include "FreeRTOS.h"
#include "FreeRTOSConfig.h"
#include <xc.h>
#include "pic32_config.h"
#include "uart.h"
#include "sysclk.h"
#include "gpio.h"
#include "uart.h"
#include "delay.h"
#include "timer.h"
#include "interrupt.h"
#include "i2c.h"
#include "rtc.h"
#include "task.h"
#include "semphr.h"

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


/* USer defines */

#define LED_RED_YELLOW  pinH6
#define LED_RED_RED     pinB13
#define LED_RED_ORANGE  pinB12
#define LED_RED_GREEN   pinA1
#define RS485_1_PIN     pinA10
#define RELAY_OUTPUT_1  pinE9
#define RELAY_OUTPUT_2  pinE8
#define RELAY_OUTPUT_3  pinA0

extern unsigned int global_tick;
static bool wdt_clear_flag = true;

void wdt_clear(void)
{
  volatile uint16_t * wdtclrkey;

  /* get address of upper 16-bit word of the WDTCON SFR */
  wdtclrkey     = ( (volatile uint16_t *)&WDTCON ) + 1;
  *wdtclrkey    = 0x5743;
}

static void print_str(const char *s)
{
  while ( *s != '\0')
    {
      uart_tx_char(PIC32_UART_4, *s++);
    }
}

static char read_char(void)
{
  if (uart_rx_any(PIC32_UART_4)) /* Data ready */
    {
      return uart_rx_char(PIC32_UART_4);
    }
  return 0;
}

void timer_2_callback(void)
{
  gpio_state_toggle(LED_RED_ORANGE);
  wdt_clear_flag = true;
}

static  char          c;
void __attribute__((vector(_UART4_RX_VECTOR), interrupt(ipl2AUTO),
                         nomips16)) _uart_4_interrupt(void)
{
  gpio_state_toggle(LED_RED_GREEN);
      c = read_char();

  IFS5bits.U4RXIF = 0;  
}

static char relay_list[3] = 
{
  RELAY_OUTPUT_1,
  RELAY_OUTPUT_2,
  RELAY_OUTPUT_3,
};

static unsigned int  millis = 0;
static SemaphoreHandle_t   semstart;
static QueueHandle_t queuoutput;


static void heartbeat(void)
{
    for (;; )
    {

      /* test read the uart */
      c = read_char();
      if (c == 'x')
        {
          if (xSemaphoreGive(semstart) == pdFALSE)
          {
            print_str("Give Failed.\n");
          }
          c = 0;
        }
      if (c == 'o')
        {
          c = read_char();

          if (xQueueSend(queuoutput, &c, pdMS_TO_TICKS(200)) == pdFALSE)
          {
            print_str("Queue send Failed.\n");
          }

          c = 0;
        }
      else if (c == '\n')
        {
          print_str("Line Feed.\n");
          c = 0;
        }

      if (wdt_clear_flag)
      {
        wdt_clear_flag = false;
        wdt_clear();
      }

      vTaskDelay(pdMS_TO_TICKS(10));
    }
}

static void togglerelay(void)
{
    for (;; )
    {

      /* test read the uart */
      if (xSemaphoreTake(semstart, pdMS_TO_TICKS(3000)))
      {
        gpio_state_toggle(LED_RED_RED);
      }
      else
      {
        gpio_state_toggle(LED_RED_YELLOW);
      }
    }
}

static void activaterelay(void)
{
    for (;; )
    {
      char ctemp;

      /* test read the uart */
      if (xQueueReceive(queuoutput, &ctemp, portMAX_DELAY))
      {
        if (ctemp - 48 < 3)
        {
          gpio_state_toggle(relay_list[ctemp-48]);
        }
      }
    }
}

int main(void)
{
  millis = interrupt_tick_get();

  sysclk_init();

  gpio_init();

  gpio_state_set(LED_RED_ORANGE, true);
  gpio_state_set(LED_RED_RED, true);
  gpio_state_set(RS485_1_PIN, true);
  gpio_state_set(RELAY_OUTPUT_1, true);
  
  /* For the interrupt */
  init_timer2(1, TMR_PRESCALE_256, 0);

  interrupt_init();

  delay_ms(1000);

  // interrupt for reception
  #if 0
  IPC42bits.U4RXIP  = 2;
  IPC42bits.U4RXIS  = 0;
  U4STAbits.URXISEL = IF_RBUF_NOT_EMPTY;
  IEC5bits.U4RXIE   = 1;
  IFS5bits.U4RXIF   = 0; 
  #endif

  uart_init(PIC32_UART_4, NO_PARITY_8_BIT_DATA, ONE_STOP_BIT, 115200);

  print_str("Hello World\n");

  i2c_init(100000);

  rtc_init();
  semstart = xSemaphoreCreateBinary();
  queuoutput = xQueueCreate(1, sizeof(char));

  /* create all tasks */
  xTaskCreate((TaskFunction_t)heartbeat, "HB", 1024, NULL, 2, NULL);

  xTaskCreate((TaskFunction_t)togglerelay, "TGL", 1024, NULL, 2, NULL);

  xTaskCreate((TaskFunction_t)activaterelay, "RA", 1024, NULL, 1, NULL);

  vTaskStartScheduler();

  /* start scheduler */

  /* */
  for(;;){}

  return 0;
}