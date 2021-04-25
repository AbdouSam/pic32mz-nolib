#include "pivot21.h"
#include "dio.h"
#include "adc_controller.h"
#include "system/tmr/sys_tmr.h"
#include "rtc_controller.h"
// we have to include libraries 
//constants 

// Digital Inputs
#define MAN_M       0  // signal to turn on the manual mode
#define INPOS       1  // signal to read any obstacle
#define INMOTOR     2  // signal to read any motor problem
#define INELEC      3  // signal to read any electrical problem
#define INWATER     4  // signal to read any water level problem
#define ECO         5  // signal of the economic mode 
// States of process
#define STATE_INIT   0 // the first state that the prrogram executes in the automatic mode
#define STATE_STOP   1 // to stop the program
#define STATE_MOV    2 // to start moving the pivot
#define STATE_IRRIG  3 // to start watering the field
#define STATE_ECO    4 // to use the economic mode 
#define STATE_NORMAL 5 // tu use the normal mode
#define STATE_PROB   6 // to indicates some issues and stop the program
#define STATE_START  7 // to manage between irrigation and movement
#define STATE_MAN    8 // to enter the manual mode
// Digital Outputs
#define MOTOR       0   // signal to turn on the motor
#define WATER       1   // signal to the pompe 
#define OUTELEC     2   // LED that indicates problem in electricity 
#define OUTMOTOR    3   // LED that indicates problem in the motor
#define OUTWATER    4   // LED that indicates lack in the water
#define OUTPOS      5   // LED that indicates an obstacle
// Analog Inputs
#define TEMP        0     // reading from temperature sensure 
#define HUM         1     // reading from humidity sensure

// add 2 more relays for marche avant , arriere
// we make sure we switch one off to open the other


// variable set

static int state;
static uint16_t temp;
static uint16_t hum;
static bool ppos;
static bool pelec;
static bool pmotor;
static bool pwater;
static bool eco ;
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

  // Read sensors
  ppos   = dio_read(INPOS); 
  pmotor = dio_read(INMOTOR);
  pelec  = dio_read(INELEC);
  pwater = dio_read(INWATER);
  eco    = dio_read(ECO) ;
  // Read current clock from RTC
  rtc_controller_getclock(&curr_clock);

  if ((man_state==true )&& (state==STATE_MAN))
    {
     state=STATE_MAN ;
    }
    else 
    {
  if (man_state == true ) // manual mode or automatic mode
  {
    state = STATE_STOP;
  }
}
  if (ppos== true || pmotor== true || pwater== true || pelec == true) // indicating a problem
  {
    state = STATE_PROB ;
  }

  switch (state)
  {
    case STATE_INIT :
    {
      //dio_turnon(4);
      //SYS_TMR_CallbackSingle (3000, 0, stp_timer_cb); to schedule  time operation
      //dio_turnoff(5);
      if (eco == true)
      {
        state= STATE_ECO ; // enter the economic mode
      }
      else
      {
        state= STATE_NORMAL ; // enter the normal mode
      }
      break;
    }
    case STATE_ECO :
    {
  int hr = curr_clock.time.hours;  // put variable hr equal to the actual hour (ask Abdallah)
  if (hr >= 22 || hr <= 5)
  {
    state=STATE_NORMAL ;  // enter the normal mode
  } 
  else
  {
   state = STATE_STOP; // enter the stop state to stop the program 
  }
      break;
  }
    
       case STATE_NORMAL :
    {
      // time 
      if ((hum< 50 && temp < 27 )||(hum< 20 && temp > 40 )) 
  {
    state=STATE_START ; // enter the start state to start the process of irrigation
  } 
  else {
    state=STATE_STOP ; // enter the stop state to stop the program 
  }

      break;
    }
      case STATE_STOP :
    {
      dio_turnoff(MOTOR);
      dio_turnoff(WATER);
      // Turn off the pivot
      if (man_state==true)
      {
        state = STATE_MAN ;
      // Enter the manual mode  
      }
      
      break;
      // to stop or enter the manual mode
    }
        case STATE_START :  
    {
        state = STATE_MOV ;
      break;
      // start movement and irrigation
    }
        case STATE_MOV :
    { 
      dio_turnon(MOTOR);
      state = STATE_IRRIG ;
      break;
      // we add some breaks
      // start movement and enter irrigation mode
    }
        case STATE_IRRIG :
    {
      dio_turnon(WATER);
      break;
      // start irrigation
    }
        case STATE_PROB :
    {
      if (ppos== true )
     dio_turnon(OUTPOS) ;
      if (pmotor== true) 
     dio_turnon(OUTMOTOR) ;
      if (pwater== true )
     dio_turnon(OUTWATER) ;
      if ( pelec == true)
     dio_turnon(OUTELEC) ;
    // turn on the right LED that indicates the type of error
     state = STATE_STOP; 
      break;
     // stop because of issues 
    }
        case STATE_MAN :
    {
      if (man_state == false)
        state= STATE_INIT ;
      break ;
      // MAnual we do nothing
    }

    default :
    {
      break;
    }
  }
}