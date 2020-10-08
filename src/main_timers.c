/*
 * @Author AbdouSam
 * @brief, simplest code to test-run a PIC32MZ without any use of libraries
 *
 */
#include <xc.h>
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

#define SYS_FREQ 200000000 // 200MHz

#define MS_SCALE    (100000)  /* Calculated for a 200MHz system */
/* Convert milliseconds to core timer ticks */
#define MS2TICKS(T) ((T) * MS_SCALE) 

static unsigned int volatile global_tick = 0;

static uint32_t volatile *gpio_lat_inv[] = 
{
  &LATAINV,
  &LATBINV
};

void toggle_pin(int port, int pin)
{
  *gpio_lat_inv[port] = 1 << pin;
}
/**
 * @brief Read current core timer, used to create delays
 *
 * @Note This exactly same as the _CP0_SET_COUNT(c) and _CP0_GET_COUN()
 * they set and get current tick from the co-processor in the mips processor.
 */
static uint32_t readcoretimer(void)
{
  volatile uint32_t timer;
  
  asm volatile("mfc0   %0, $9" : "=r"(timer));
  
  return timer;
}

static void setcoretime(volatile uint32_t count)
{
  asm volatile("mtc0    %0, $9" : :"r"(count));
}

/* Same as delay_us uses inline assembly for showing purposes */
void delay_us_2(uint32_t us)
{
  us *= SYS_FREQ / 1000000 / 2; //Core timer updates every 2 ticks
  setcoretime(0);

  while (us > readcoretimer());
}

 void __attribute__((unused)) delay_us(uint32_t us) 
{
  us *= SYS_FREQ / 1000000 / 2; //Core timer updates every 2 ticks

  _CP0_SET_COUNT(0); // Set core timer count to 0

  while (us > _CP0_GET_COUNT());
}

void delay_ms(uint32_t ms)
{
  delay_us_2(1000 * ms);
}

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

/* Set the clocks for different peripherals */
void set_performance_mode()
{
  unsigned int cp0;

  //Unlock sequence
  asm volatile("di"); // disable all interrupts
  SYSKEY = 0xAA996655;
  SYSKEY = 0x556699AA;

  //PB1DIV
  //Peripheral Bus 1 cannot be turned off, so there's no need to turn it on
  PB1DIVbits.PBDIV = PERPHERAL1_CLK_DIV; //Peripheral Bus 1 Clock Divisor Control (PBCLK1 is SYSCLK / 2)
  
  //PB2DIV
  PB2DIVbits.PBDIV = PERPHERAL2_CLK_DIV; //Peripheral Bus 2 Clock Divisor Control (PBCLK2 is SYSCLK / 2)
  PB2DIVbits.ON = 1; //Peripheral Bus 2 Output Clock Enable

  PB3DIVbits.PBDIV = PERPHERAL3_CLK_DIV;
  PB3DIVbits.ON = 1; 

  PB4DIVbits.PBDIV = PERPHERAL4_CLK_DIV; 
  PB4DIVbits.ON = 1; 

  PB5DIVbits.PBDIV = PERPHERAL5_CLK_DIV; 
  PB5DIVbits.ON = 1; 

  PB7DIVbits.PBDIV = PERPHERAL7_CLK_DIV; 
  PB7DIVbits.ON = 1; 

  PB8DIVbits.PBDIV = PERPHERAL8_CLK_DIV; 
  PB8DIVbits.ON = 1; 

  // PRECON - Set up prefetch
  PRECONbits.PFMSECEN = 0; // Flash SEC Interrrupt Enable ( Do not generate interrupt when PFMSECbit is set)
  PRECONbits.PREFEN = 0b11; // Predictive prefech Enable (for any address)
  PRECONbits.PFMWS  = 0b010; // PFM Access Time Defined in Terms of SYSCLK Wait States

  // Set up caching -system control coprocessor- register number 16
  cp0 = _mfc0(16, 0);
  cp0 &= ~0x07;
  cp0 |= 0b011; // K0 = Cacheable, non-coherent, write-back, write-allocate
  _mtc0(16, 0, cp0);

  // Lock Sequence
  SYSKEY = 0x33333333;
  asm volatile("ei"); // enable interrupts

}

uint32_t sysclk_timerfreq_get(void)
{
  return (SYS_FREQ / (PERPHERAL3_CLK_DIV + 1));
}

uint32_t sysclk_uartfreq_get(void)
{
  return (SYS_FREQ / (PERPHERAL2_CLK_DIV + 1));
}

void _putc4(char c)
{
  while (U4STAbits.UTXBF);
  U4TXREG = c;
}

void print_str4(char *s)
{
  LATASET = 1 << 10;
  delay_ms(10);

  while ( *s != '\0')
  {
    _putc4(*s++);
  }


}

void UART4_init(int baud_rate)
{
  int pb_uart_clk = sysclk_uartfreq_get();

  // Set rs485 bit to output
  TRISACLR = 1 << 10;
  // Set up the UART 6
  // RX to RPF2
  U4RXR = 0b0010;
  TRISBbits.TRISB14 = 1; //set its IO to input

  // TX to RPF3
  RPF12R = 0b0010;

  U4MODE = 0; // Set UART 5 off, prior to setting it up.
  U4MODEbits.BRGH = 0; // standard speed mode, included in the previsous statmem

  // formulat straight from datasheet.
  U4BRG = pb_uart_clk / ( 16  * baud_rate) - 1; 
  U4STA = 0; // disable Tx Rx, clear all flags
  U4STAbits.UTXEN = 1;
  U4STAbits.URXEN = 1;

  U4MODEbits.PDSEL = 0; // controls how many data bits parity,... we chose default
  U4MODEbits.STSEL = 0; // stop bit we chose 1
  
  U4MODEbits.ON = 1; // Turn ON

  print_str4("Hello UART \n");
}


char read_char4(void)
{
  if (U4STAbits.URXDA) // Data ready
  {
    return U4RXREG;
  }
  return 0;
}

// initialize timer. timers are ticked with the PBCLK3. the Value in TMR is compared
// PR (Period Register)
void init_timer2(int period)
{
  T2CON = 0x0; // disable timer 2
  TMR2 = 0; // Timer counter

  IEC0bits.T2IE = 0; //disable timer 2 interrupt
  T2CONbits.TCKPS = 0b011; // prescale 8

  // To get 1khz from 100 MHz the value of PR should be 100000 > 65535, we use a prescaler
  // of 8, we check if it is available for timer 2
  uint32_t tmr_clk = sysclk_timerfreq_get();

  PR2 = tmr_clk / period / 8;


  IFS0bits.T2IF = 0; //clear flag
  IPC2bits.T2IP = 3; // periority 3
  IPC2bits.T2IS = 1; // sub-periority 1
  IEC0bits.T2IE = 1; // enable interrupt

  T2CONbits.TON  = 1; // turnon


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
void __attribute__((vector(_TIMER_2_VECTOR), interrupt(ipl3AUTO), nomips16)) _timer2_interrupt(void)
{
  IFS0bits.T2IF = 0;
  global_tick+=1;
}

void __attribute__((vector(_TIMER_1_VECTOR), interrupt(ipl3AUTO), nomips16)) _timer1_interrupt(void)
{
  IFS0bits.T1IF = 0;
  global_tick+=1;
}

int main(void)
{

  set_performance_mode();

  ANSELB = 0; // PIC starts all pins as analog, needs to be deactivated to be used
  TRISBCLR = 1 << 13; //set bit 13 port B to output
  TRISBCLR = 1 << 12; //set bit 12 port B to output

  LATBSET = 1 <<  13; // Set bit 13 for Port B to 1
  LATBSET = 1 <<  12; // Set bit 12 for Port B to 1

  // For the interrupt

  INTCONbits.MVEC = 1; // this enables the multi Vectored interrupt to tell the microprocessor
  // to allow different handlers for each different type of interrupt (timer2, timer 3)
  init_timer1(1000);

  UART4_init(115200);

  unsigned int millis = global_tick;

  for (;; )
    {

      if (global_tick - millis >= 1000)
      {
        toggle_pin(1, 12);
        toggle_pin(1, 13);
        millis = global_tick;
      }
    }

  return 0;
}