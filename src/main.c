#include <xc.h>

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
#pragma config FUSBIDIO =   ON

/*** BF1SEQ0 ***/

#pragma config TSEQ =       0x0000
#pragma config CSEQ =       0xffff

#if 0

const DRV_USART_INIT drvUsart2InitData =
{
  .moduleInit.value     = DRV_USART_POWER_STATE_IDX2,
  .usartID              = DRV_USART_PERIPHERAL_ID_IDX2,
  .mode                 = DRV_USART_OPER_MODE_IDX2,
  .flags                = DRV_USART_INIT_FLAGS_IDX2,
  .brgClock             = DRV_USART_BRG_CLOCK_IDX2,
  .lineControl          = DRV_USART_LINE_CNTRL_IDX2,
  .baud                 = DRV_USART_BAUD_RATE_IDX2,
  .handshake            = DRV_USART_HANDSHAKE_MODE_IDX2,
  .interruptTransmit    = DRV_USART_XMIT_INT_SRC_IDX2,
  .interruptReceive     = DRV_USART_RCV_INT_SRC_IDX2,
  .interruptError       = DRV_USART_ERR_INT_SRC_IDX2,
  .queueSizeTransmit    = DRV_USART_XMIT_QUEUE_SIZE_IDX2,
  .queueSizeReceive     = DRV_USART_RCV_QUEUE_SIZE_IDX2,
};

const DRV_TMR_INIT drvTmr0InitData =
{
  .moduleInit.sys.powerState    = DRV_TMR_POWER_STATE_IDX0,
  .tmrId                        = DRV_TMR_PERIPHERAL_ID_IDX0,
  .clockSource                  = DRV_TMR_CLOCK_SOURCE_IDX0,
  .prescale                     = DRV_TMR_PRESCALE_IDX0,
  .mode                         = DRV_TMR_OPERATION_MODE_IDX0,
  .interruptSource              = DRV_TMR_INTERRUPT_SOURCE_IDX0,
  .asyncWriteEnable             = false,
};

void sys_init(void)
{
  printf("Init sys\n");

  SYS_CLK_Initialize( NULL );
  SYS_DEVCON_PerformanceConfig(SYS_CLK_SystemFrequencyGet());
  SYS_DEVCON_JTAGEnable();
  SYS_PORTS_Initialize();

  /*** Interrupt Service Initialization Code ***/
  SYS_INT_Initialize();

  /* Initialize Drivers */

  sysObj.drvTmr0 = DRV_TMR_Initialize(DRV_TMR_INDEX_0,
                                      (SYS_MODULE_INIT*)&drvTmr0InitData);
  SYS_INT_VectorPrioritySet(DRV_TMR_INTERRUPT_VECTOR_IDX0,
                            DRV_TMR_INTERRUPT_PRIORITY_IDX0);
  SYS_INT_VectorSubprioritySet(DRV_TMR_INTERRUPT_VECTOR_IDX0,
                               DRV_TMR_INTERRUPT_SUB_PRIORITY_IDX0);


  /* USART 0 Initialization */

  sysObj.drvUsart2 = DRV_USART_Initialize(DRV_USART_INDEX_2,
                                         (SYS_MODULE_INIT*)&drvUsart2InitData);
  SYS_INT_VectorPrioritySet(INT_VECTOR_UART6_TX, DRV_USART_XMIT_INT_PRIORITY_IDX2);
  SYS_INT_VectorSubprioritySet(INT_VECTOR_UART6_TX, DRV_USART_XMIT_INT_SUBPRIORITY_IDX2);
  SYS_INT_VectorPrioritySet(INT_VECTOR_UART6_RX, DRV_USART_RCV_INT_PRIORITY_IDX2);
  SYS_INT_VectorSubprioritySet(INT_VECTOR_UART6_RX, DRV_USART_RCV_INT_SUBPRIORITY_IDX2);
  SYS_INT_VectorPrioritySet(INT_VECTOR_UART6_FAULT, DRV_USART_ERR_INT_PRIORITY_IDX2);
  SYS_INT_VectorSubprioritySet(INT_VECTOR_UART6_FAULT, DRV_USART_ERR_INT_SUBPRIORITY_IDX2);


  SYS_INT_Enable();
}

#endif

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
  PB2DIVbits.PBDIV = 1; //Peripheral Bus 2 Clock Divisor Control (PBCLK1 is SYSCLK / 2)
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

int main(void)
{

  set_performance_mode();

  ANSELB = 0; // PIC starts all pins as analog, needs to be deactivated to be used
  TRISBCLR = 1 << 13; //set bit 0 port A to output
  TRISBCLR = 1 << 12; //set bit 0 port A to output

  LATBSET = 1 <<  13; // Set bit 0 for Port A to 1
  LATBSET = 1 <<  12; // Set bit 0 for Port A to 1

  for (;; )
    {
    }

  return 0;
}