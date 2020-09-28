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

void __attribute__(("unused")) delay_us(uint32_t us)
{
  us *= SYS_FREQ / 1000000 / 2; //Core timer updates every 2 ticks

  _CP0_SET_COUNT(0); // Set core timer count to 0

  while (us > _CP0_GET_COUNT());
}

void delay_ms(uint32_t ms)
{
  delay_us_2(1000 * ms);
}

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
  PB1DIVbits.PBDIV = 1; //Peripheral Bus 1 Clock Divisor Control (PBCLK1 is SYSCLK / 2)
  
  //PB2DIV
  PB2DIVbits.PBDIV = 1; //Peripheral Bus 2 Clock Divisor Control (PBCLK2 is SYSCLK / 2)
  PB2DIVbits.ON = 1; //Peripheral Bus 2 Output Clock Enable

  PB3DIVbits.PBDIV = 4;
  PB3DIVbits.ON = 1; 

  PB4DIVbits.PBDIV = 1; 
  PB4DIVbits.ON = 1; 

  PB5DIVbits.PBDIV = 1; 
  PB5DIVbits.ON = 1; 

  PB7DIVbits.PBDIV = 0; 
  PB7DIVbits.ON = 1; 

  PB8DIVbits.PBDIV = 1; 
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

/**
 * UART Routines
 */
void _putc6(char c)
{
  while (U6STAbits.UTXBF);
  U6TXREG = c;
}

void _putc4(char c)
{
  while (U4STAbits.UTXBF);
  U4TXREG = c;
}

void print_str6(char *s)
{
  while ( *s != '\0')
  {
    _putc6(*s++);
  }
}


void print_str4(char *s)
{
  LATASET = 1 << 10;
  delay_ms(10);

  while ( *s != '\0')
  {
    _putc4(*s++);
  }

  delay_ms(50);
  LATACLR = 1 << 10;
}

void UART6_init(int baud_rate)
{
  int pb_uart_clk = SYS_FREQ / 2;

  // Set up the UART 6
  // RX to RPF2
  U6RXR = 0b1011;
  TRISFbits.TRISF2 = 1; //set its IO to input

  // TX to RPF3
  RPF3R = 0b0100;
  TRISFbits.TRISF3 = 0; // Set its IO to output

  U6MODE = 0; // Set UART 5 off, prior to setting it up.
  U6MODEbits.BRGH = 0; // standard speed mode, included in the previsous statmem

  // formulat straight from datasheet.
  U6BRG = pb_uart_clk / ( 16  * baud_rate) - 1; 
  U6STA = 0; // disable Tx Rx, clear all flags
  U6STAbits.UTXEN = 1;
  U6STAbits.URXEN = 1;

  U6MODEbits.PDSEL = 0; // controls how many data bits parity,... we chose default
  U6MODEbits.STSEL = 0; // stop bit we chose 1
  
  U6MODEbits.ON = 1; // Turn ON

  print_str6("Hello UART \n");
}


void UART4_init(int baud_rate)
{
  int pb_uart_clk = SYS_FREQ / 2;

  // Set rs485 bit to output
  ANSELA = 0;
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

int main(void)
{

  set_performance_mode();

  ANSELB = 0; // PIC starts all pins as analog, needs to be deactivated to be used
  TRISBCLR = 1 << 13; //set bit 13 port B to output
  TRISBCLR = 1 << 12; //set bit 12 port B to output

  LATBSET = 1 <<  13; // Set bit 13 for Port B to 1
  LATBSET = 1 <<  12; // Set bit 12 for Port B to 1

  int delay_time = 1000;
  char c;

  UART4_init(19200);

  for (;; )
    {
      delay_ms(delay_time);
      LATBINV = 1 << 13;
      LATBINV = 1 << 12;

      c = 0;
      c = read_char4();

      if (c == 'u')
      {
        delay_time+=500;
      }
      else if (c == 'd')
      {
        delay_time-=500;
      }

      if (delay_time > 5000)
      {
        delay_time = 5000;
      }
      else if (delay_time < 500)
      {
        delay_time = 500;
      }
      print_str4("Hello UART \n");
    }

  return 0;
}