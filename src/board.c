#include "board.h"
#include <xc.h>

void led_init(void)
{
  ANSELB = 0; // PIC starts all pins as analog, needs to be deactivated to be used
  TRISBCLR = 1 << 13; //set bit 13 port B to output
  TRISBCLR = 1 << 12; //set bit 12 port B to output
}

void led_state(int led, int state)
{
  if (state == 0)
  {
    LATBSET = 1 << led;
  }
  else
  {
    LATBCLR = 1 << led;
  }
}
void led_toggle(int led)
{
  LATBINV = 1 << led;
}