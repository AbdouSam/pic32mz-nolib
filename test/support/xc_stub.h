#ifndef XC_STUB_H
#define XC_STUB_H

/**
 * Stub for GPIO register to unit test the module
 */
#if (PIC32_PIN_COUNT != 64)

extern volatile uint32_t PORTA;
extern volatile uint32_t TRISA;
extern volatile uint32_t LATA;
extern volatile uint32_t ANSELA;
extern volatile uint32_t ANSELC;
extern volatile uint32_t ANSELF;

#endif

extern volatile uint32_t PORTB;
extern volatile uint32_t TRISB;
extern volatile uint32_t LATB;
extern volatile uint32_t ANSELB;
extern volatile uint32_t PORTC;
extern volatile uint32_t TRISC;
extern volatile uint32_t LATC;
extern volatile uint32_t PORTD;
extern volatile uint32_t TRISD;
extern volatile uint32_t LATD;
extern volatile uint32_t PORTE;
extern volatile uint32_t TRISE;
extern volatile uint32_t LATE;
extern volatile uint32_t ANSELE;
extern volatile uint32_t PORTF;
extern volatile uint32_t TRISF;
extern volatile uint32_t LATF;
extern volatile uint32_t PORTG;
extern volatile uint32_t TRISG;
extern volatile uint32_t LATG;
extern volatile uint32_t ANSELG;

#if ((PIC32_PIN_COUNT != 64) && (PIC32_PIN_COUNT != 100))

extern volatile uint32_t PORTH;
extern volatile uint32_t TRISH;
extern volatile uint32_t LATH;
extern volatile uint32_t ANSELH   ;
extern volatile uint32_t ANSELJ   ;
extern volatile uint32_t PORTJ;
extern volatile uint32_t TRISJ;
extern volatile uint32_t LATJ;
extern volatile uint32_t ANSELD;

#endif

#if ((PIC32_PIN_COUNT != 64) && (PIC32_PIN_COUNT != 100) && \
  (PIC32_PIN_COUNT != 124))

extern volatile uint32_t PORTK;
extern volatile uint32_t TRISK;
extern volatile uint32_t LATK;

#endif


extern volatile uint32_t RPD2R;
extern volatile uint32_t RPG8R;
extern volatile uint32_t RPF4R;
extern volatile uint32_t RPD10R;
extern volatile uint32_t RPF1R;
extern volatile uint32_t RPB9R;
extern volatile uint32_t RPB10R;
extern volatile uint32_t RPC14R;
extern volatile uint32_t RPB5R;

#if (PIC32_PIN_COUNT != 64)
extern volatile uint32_t RPC1R;
extern volatile uint32_t RPD14R;
extern volatile uint32_t RPG1R;
extern volatile uint32_t RPA14R;
#endif

#if ((PIC32_PIN_COUNT != 100) && (PIC32_PIN_COUNT != 64))
extern volatile uint32_t RPD6R;
#endif

extern volatile uint32_t RPD3R;
extern volatile uint32_t RPG7R;
extern volatile uint32_t RPF5R;
extern volatile uint32_t RPD11R;
extern volatile uint32_t RPF0R;
extern volatile uint32_t RPB1R;
extern volatile uint32_t RPE5R;
extern volatile uint32_t RPC13R;
extern volatile uint32_t RPB3R;

#if (PIC32_PIN_COUNT != 64)
extern volatile uint32_t RPC4R;
extern volatile uint32_t RPD15R;
extern volatile uint32_t RPG0R;
extern volatile uint32_t RPA15R;
#endif

#if ((PIC32_PIN_COUNT != 100) && (PIC32_PIN_COUNT != 64))
extern volatile uint32_t RPD7R;
#endif

extern volatile uint32_t RPD9R;
extern volatile uint32_t RPG6R;
extern volatile uint32_t RPB8R;
extern volatile uint32_t RPB15R;
extern volatile uint32_t RPD4R;
extern volatile uint32_t RPB0R;
extern volatile uint32_t RPE3R;
extern volatile uint32_t RPB7R;

#if (PIC32_PIN_COUNT != 64)
extern volatile uint32_t RPF12R;
extern volatile uint32_t RPD12R;
extern volatile uint32_t RPF8R;
extern volatile uint32_t RPC3R;
extern volatile uint32_t RPE9R;
#endif

extern volatile uint32_t RPD1R;
extern volatile uint32_t RPG9R;
extern volatile uint32_t RPB14R;
extern volatile uint32_t RPD0R;
extern volatile uint32_t RPB6R;
extern volatile uint32_t RPD5R;
extern volatile uint32_t RPB2R;
extern volatile uint32_t RPF3R;

#if (PIC32_PIN_COUNT != 64)
extern volatile uint32_t RPF13R;
extern volatile uint32_t RPF2R;
extern volatile uint32_t RPC2R;
extern volatile uint32_t RPE8R;
#endif

#endif //XC_STUB_H