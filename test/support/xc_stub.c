#include <stdint.h>
#include "pic32_config.h"

/**
 * Stub for GPIO register to unit test the module
 */

#if (PIC32_PIN_COUNT != 64)

volatile uint32_t PORTA = 0;
volatile uint32_t TRISA = 0;
volatile uint32_t LATA = 0;
volatile uint32_t ANSELA = 0;
volatile uint32_t ANSELC = 0;
volatile uint32_t ANSELF = 0;

#endif

volatile uint32_t PORTB = 0;
volatile uint32_t TRISB = 0;
volatile uint32_t LATB = 0;
volatile uint32_t ANSELB = 0;
volatile uint32_t PORTC = 0;
volatile uint32_t TRISC = 0;
volatile uint32_t LATC = 0;
volatile uint32_t PORTD = 0;
volatile uint32_t TRISD = 0;
volatile uint32_t LATD = 0;
volatile uint32_t PORTE = 0;
volatile uint32_t TRISE = 0;
volatile uint32_t LATE = 0;
volatile uint32_t ANSELE = 0;
volatile uint32_t PORTF = 0;
volatile uint32_t TRISF = 0;
volatile uint32_t LATF = 0;
volatile uint32_t PORTG = 0;
volatile uint32_t TRISG = 0;
volatile uint32_t LATG = 0;
volatile uint32_t ANSELG = 0;

#if ((PIC32_PIN_COUNT != 64) && (PIC32_PIN_COUNT != 100))

volatile uint32_t PORTH = 0;
volatile uint32_t TRISH = 0;
volatile uint32_t LATH = 0;
volatile uint32_t ANSELH    = 0;
volatile uint32_t ANSELJ    = 0;
volatile uint32_t PORTJ = 0;
volatile uint32_t TRISJ = 0;
volatile uint32_t LATJ = 0;
volatile uint32_t ANSELD = 0;

#endif

#if ((PIC32_PIN_COUNT != 64) && (PIC32_PIN_COUNT != 100) && \
  (PIC32_PIN_COUNT != 124))

volatile uint32_t PORTK = 0;
volatile uint32_t TRISK = 0;
volatile uint32_t LATK = 0;

#endif


volatile uint32_t RPD2R = 0;
volatile uint32_t RPG8R = 0;
volatile uint32_t RPF4R = 0;
volatile uint32_t RPD10R = 0;
volatile uint32_t RPF1R = 0;
volatile uint32_t RPB9R = 0;
volatile uint32_t RPB10R = 0;
volatile uint32_t RPC14R = 0;
volatile uint32_t RPB5R = 0;

#if (PIC32_PIN_COUNT != 64)
volatile uint32_t RPC1R = 0;
volatile uint32_t RPD14R = 0;
volatile uint32_t RPG1R = 0;
volatile uint32_t RPA14R = 0;
#endif

#if ((PIC32_PIN_COUNT != 100) && (PIC32_PIN_COUNT != 64))
volatile uint32_t RPD6R = 0;
#endif

volatile uint32_t RPD3R = 0;
volatile uint32_t RPG7R = 0;
volatile uint32_t RPF5R = 0;
volatile uint32_t RPD11R = 0;
volatile uint32_t RPF0R = 0;
volatile uint32_t RPB1R = 0;
volatile uint32_t RPE5R = 0;
volatile uint32_t RPC13R = 0;
volatile uint32_t RPB3R = 0;

#if (PIC32_PIN_COUNT != 64)
volatile uint32_t RPC4R = 0;
volatile uint32_t RPD15R = 0;
volatile uint32_t RPG0R = 0;
volatile uint32_t RPA15R = 0;
#endif

#if ((PIC32_PIN_COUNT != 100) && (PIC32_PIN_COUNT != 64))
volatile uint32_t RPD7R = 0;
#endif

volatile uint32_t RPD9R = 0;
volatile uint32_t RPG6R = 0;
volatile uint32_t RPB8R = 0;
volatile uint32_t RPB15R = 0;
volatile uint32_t RPD4R = 0;
volatile uint32_t RPB0R = 0;
volatile uint32_t RPE3R = 0;
volatile uint32_t RPB7R = 0;

#if (PIC32_PIN_COUNT != 64)
volatile uint32_t RPF12R = 0;
volatile uint32_t RPD12R = 0;
volatile uint32_t RPF8R = 0;
volatile uint32_t RPC3R = 0;
volatile uint32_t RPE9R = 0;
#endif

volatile uint32_t RPD1R = 0;
volatile uint32_t RPG9R = 0;
volatile uint32_t RPB14R = 0;
volatile uint32_t RPD0R = 0;
volatile uint32_t RPB6R = 0;
volatile uint32_t RPD5R = 0;
volatile uint32_t RPB2R = 0;
volatile uint32_t RPF3R = 0;

#if (PIC32_PIN_COUNT != 64)
volatile uint32_t RPF13R = 0;
volatile uint32_t RPF2R = 0;
volatile uint32_t RPC2R = 0;
volatile uint32_t RPE8R = 0;
#endif

