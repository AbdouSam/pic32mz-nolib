#include "pivot21.h"
// we have to include libraries
#include "dio.h"
#include "adc_controller.h"
#include "system/tmr/sys_tmr.h"
#include "rtc_controller.h"

//constants 

// Digital Inputs
#define MAN_M       1 
#define POS         0

// States of process
#define STATE_INIT  0 
#define STATE_STOP  3 
#define STATE_MOV   1
#define STATE_IRRIG 2

// Analog Inputs
#define TEMP        0
#define HUM         1

// add 2 more relays for marche avant , arriere
// we make sure we switch one off to open the other
// variable set

static int state;
static uint16_t temp;
static uint16_t hum;
static bool pos;
static rtc_clock curr_clock;

void stp_timer_cb(void)
{
  state = STATE_STOP;

  /*
  int hr = curr_clock.time.hour;
  if (hr >= 22 || hr <= 5)
  {
  } 
  */
}

void pivot21_init(void)
{
  state = STATE_INIT;
}

void pivot21_task(void)
{ 
  // Read temperature
  adc_controller_readadc(TEMP, &temp);
  
  // Read humidity
  adc_controller_readadc(HUM, &hum);

  // read state of manual/auto button
  bool man_state = dio_read(MAN_M);

  // Read position sensor
  pos = dio_read(POS); //state = stop

  // Read current clock from RTC
  rtc_controller_getclock(&curr_clock); 

  
  if (man_state == true ) 
  {
    state = STATE_STOP;
  }
  else 
  {
    
  } 

  switch (state)
  {
    case STATE_INIT :
    {
      dio_turnon(4);
      SYS_TMR_CallbackSingle (3000, 0, stp_timer_cb);
      dio_turnoff(5);
      state = STATE_MOV;
      break;
    }

    case STATE_STOP :
    {
      dio_turnon(5);
      dio_turnoff(4);
      break;
    }

    default :
    {

      break;
    }
  }
}