#include "pivot21.h"
#include "dio.h"
#include "adc_controller.h"
#include "system/tmr/sys_tmr.h"
#include "rtc_controller.h"
#include "debug.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* we have to include libraries */
/*constants */

/* Digital Inputs*/
#define MAN_M       0  /* signal to turn on the manual mode*/
#define INPOS       1  /* signal to read any obstacle*/
#define INMOTOR     2  /* signal to read any motor problem*/
#define INELEC      3  /* signal to read any electrical problem*/
#define INWATER     4  /* signal to read any water level problem*/
#define ECO         5  /* signal of the economic mode */

/* States of process*/
#define STATE_INIT   0 /* the first state that the prrogram executes in the automatic mode*/
#define STATE_NORMAL 1 /* tu use the normal mode*/
#define STATE_START  2 /* to manage between irrigation and movement*/
#define STATE_STOP   3 /* to stop the program*/
#define STATE_MOV    4 /* to start moving the pivot*/
#define STATE_IRRIG  5 /* to start watering the field*/
#define STATE_PROB   6 /* to indicates some issues and stop the program*/
#define STATE_MEH   (19) /* to enter the manual mode*/
/* #define STATE_MAN    8  to enter the manual mode*/
/* #define STATE_ECO    4 to use the economic mode */

#define TMAX         (80)  /* reading the max value of the sensor*/
#define TMIN         (-20)   /* reading the min value OF THE SENSOR*/
/* #define MINUTE       (60000) csnt for one minute */

/* Digital Outputs*/
#define MOTOR_FWD   0   /* signal to turn on the motor */
#define MOTOR_BWD   7   /* signal to turn on the motor */
#define WATER       1   /* signal to the pompe */
#define OUTELEC     2   /* LED that indicates problem in electricity */
#define OUTMOTOR    3   /* LED that indicates problem in the motor */
#define OUTWATER    4   /* LED that indicates lack in the water */
#define OUTPOS      5   /* LED that indicates an obstacle */
#define DIRECTION   6   /* SIGNAL that indicates the direction */


/* Analog Inputs*/
#define TEMP        0     /* reading from temperature sensure */
#define HUM         1     /* reading from humidity sensure */

#define DIR_FWD          0     /* reading from humidity sensure */
#define DIR_BWD          1     /* reading from humidity sensure */


/* Default values for timing */

#define TIME_ON_DEFAULT     (60U) /* S */
#define TIME_IRRIG_DEFAULT  (45U) /* S */
#define TIME_MOVE_DEFAULT   (15U) /* S */
#define ECO_MODE_DEFAULT    (false)

/* add 2 more relays for marche avant , arriere*/

/* variable set*/

static int state;
static int temp;
static int hum;
static bool ppos;
static bool pelec;
static bool pmotor;
static bool pwater;
static bool eco  = ECO_MODE_DEFAULT;
static bool man_state;
static bool new_ppos = true ;
static rtc_clock curr_clock;
static bool state_entery = true ;
static uint32_t time_on = TIME_ON_DEFAULT;
static uint32_t time_irrig = TIME_IRRIG_DEFAULT;  
static uint32_t time_move = TIME_MOVE_DEFAULT;
static uint32_t start_time;
static uint32_t curr_time;
static bool read_start_time = true ;
static int time_cond;
static bool init_cond = false;
static SYS_TMR_HANDLE time_move_handle;
static SYS_TMR_HANDLE time_irrig_handle;
static SYS_TMR_HANDLE ppos_handle;
static int curr_hr;
static int motor_direction = DIR_FWD;

/* static bool start_timer_mov = true ; this should be connected to a bool pin "once f denia used"*/



/***********************************************
 * 
 ***********************************************/

typedef struct
{
  int start;  
  int size;  
}parse_data_t;

int pivot21_set_parameter(const char *frame)
{
  char timeduratoin_buffer[10];
  char irrigation_time_buffer[10];
  char moving_time_buffer[10];
  int i;

  int timeduratoin;
  int irrigation_time;
  int moving_time;
  bool economic_mode;

  parse_data_t timeduratoin_pdata = {0, 0};
  parse_data_t irrigation_time_pdata = {0, 0};
  parse_data_t moving_time_pdata = {0, 0};

  int economic_mode_pos = 0;

  for (i = 4; i < 50; i++)
  {
    if (frame[i] == 'D')
    {
      timeduratoin_pdata.start = i + 1;
    }
    else if (frame[i] == 'I')
    {
      timeduratoin_pdata.size = i - timeduratoin_pdata.start;
      irrigation_time_pdata.start = i + 1;
    }
    else if (frame[i] == 'M')
    {
      irrigation_time_pdata.size = i  - irrigation_time_pdata.start;
      moving_time_pdata.start = i + 1;
    }
    else if (frame[i] == 'E')
    {
      moving_time_pdata.size = i - moving_time_pdata.start;
      economic_mode_pos = i + 1;
      /* End of parsing */
      break;
    }
  }

  /* conversion from string to int 
   * Check if parameters are mispositioned 
   */
  if ((timeduratoin_pdata.start > 6 || timeduratoin_pdata.size >= 10 ) ||
     (irrigation_time_pdata.start > 16 || irrigation_time_pdata.size >= 10 ) ||
     (moving_time_pdata.start > 26 || moving_time_pdata.size >= 10 ))
  {
    app_dbg_msg("APP: Error receiving parameters\n");
    return -1;
  }

  /* copy data from received frame to local buffer for convertsion */

  memcpy(timeduratoin_buffer, &frame[timeduratoin_pdata.start], timeduratoin_pdata.size);
  timeduratoin_buffer[timeduratoin_pdata.size] = '\0';
  timeduratoin = atoi(timeduratoin_buffer);
  
  if (timeduratoin == 0)
    {
      timeduratoin = TIME_ON_DEFAULT;
    }

  memcpy(irrigation_time_buffer, &frame[irrigation_time_pdata.start], irrigation_time_pdata.size);
  irrigation_time_buffer[irrigation_time_pdata.size] = '\0';
  irrigation_time = atoi(irrigation_time_buffer);
  
  if (irrigation_time == 0)
    {
      irrigation_time = TIME_IRRIG_DEFAULT;
    }

  memcpy(moving_time_buffer, &frame[moving_time_pdata.start], moving_time_pdata.size);
  moving_time_buffer[moving_time_pdata.size] = '\0';
  moving_time = atoi(moving_time_buffer);

  if (moving_time == 0)
    {
      moving_time = TIME_MOVE_DEFAULT;
    }

  if (frame[economic_mode_pos] == '1')
    {
      economic_mode = true;
    }
  else if (frame[economic_mode_pos] == '0')
    {
      economic_mode = false;
    }
  else
    {
      economic_mode = ECO_MODE_DEFAULT;
    }

  time_on     = timeduratoin;
  time_irrig  = irrigation_time;
  time_move   = moving_time;
  eco         = economic_mode;

  app_dbg_msg("APP: param received:\n");
  app_dbg_msg("- duration: %d S\n",   timeduratoin);
  app_dbg_msg("- irrig time: %d S\n", irrigation_time);
  app_dbg_msg("- move time: %d S\n",  moving_time);
  app_dbg_msg("- eco mode %s\n",      (economic_mode == true) ? "ENABLED" : "DISABLED");

  /* Start the cycle when we receive data */
  init_cond = true;
  read_start_time = true;

  return 0;
}


int pivot21_set_parameter_default(void)
{
  time_on     = TIME_ON_DEFAULT;
  time_irrig  = TIME_IRRIG_DEFAULT;
  time_move   = TIME_MOVE_DEFAULT;
  eco         = ECO_MODE_DEFAULT;
  init_cond = true;
  read_start_time = true;

  app_dbg_msg("APP: param set Default:\n");
  return 0;
}

int pivot21_read_parameter(const char *frame, char *retframe)
{
  /* The frame received has this form: @00prX, where the X can be either:
   * T: for temperature
   * H: for humidity
   * M: for Manual Mode
   * F: for Failures
   * 0: Position Faiure 
   * 1: Motor Failure
   * 2: Electric Failure
   * 3: Water Failure
   *
   * Parse with data is requested depending on the fifth character in the frame.
   */

  app_dbg_msg("APP: Request Parameter %c\n", frame[5]);

  switch(frame[5])
  {
    case 'T':
    {
      sprintf(retframe, "%d", temp);
      break;
    }

    case 'H':
    {
      sprintf(retframe, "%d", hum);
      break;
    }

    case 'M':
    {
      sprintf(retframe, "%d", man_state);
      break;
    }

    case 'F':
    {
      switch(frame[6])
      {
        case '0':
        {
          sprintf(retframe, "%d", ppos);
          break;
        }

        case '1':
        {
          sprintf(retframe, "%d", pmotor);
          break;
        }

        case '2':
        {
          sprintf(retframe, "%d", pelec);
          break;
        }
        
        case '3':
        {
          sprintf(retframe, "%d", pwater);
          break;
        }
      }

      sprintf(retframe, "%d", hum);
      break;
    }

    default:
    {

      sprintf(retframe, "%d", -1);
      break;
    }
  }

  return 0;
}

void set_state (int st)
{
  state = st;
  state_entery = true;
}
/* functions */

static bool is_weather_valid(void)
{
  return ((hum < 50 && temp < 27 ) || (hum < 20 && temp > 40 ));
}

static bool is_time_valid(void)
{
  return (time_cond <= time_on);
}
static bool is_eco_time(void)
{
  return (curr_hr >= 22 || curr_hr <= 5);
}
static bool check_failures(void)
{
  return ( /* ppos == true || */ pmotor == true || pwater == true || pelec == true);
}

void irrig_done ( uintptr_t context, uint32_t currTick )
{
  dio_turnoff(WATER) ;
  set_state(STATE_MOV);
  app_dbg_msg("I am done irrigating! \n ");

}
void disp_done ( uintptr_t context, uint32_t currTick )
{ 
   dio_turnoff(MOTOR_FWD) ;
   dio_turnoff(MOTOR_BWD) ;
   set_state(STATE_IRRIG);
   app_dbg_msg("I am done moving! \n ");
}

void ppos_done(uintptr_t context, uint32_t cuurTick)
{
  /* Recheck ppos after 5 seconds */
  if (ppos)
  {
     new_ppos = true;

     if (motor_direction == DIR_FWD)
        { 
          app_dbg_msg("Position problem confirmed, Turning Backward.\n");
          dio_turnoff(MOTOR_FWD);
          dio_turnon(MOTOR_BWD);
        }
      else
        {
          app_dbg_msg("Position problem confirmed, Turning Forward.\n");
          dio_turnoff(MOTOR_BWD);
          dio_turnon(MOTOR_FWD);
        }
      
        motor_direction = (motor_direction == DIR_FWD) ? DIR_BWD : DIR_FWD;
  }

}
/*
void stp_timer_cb( uintptr_t context, uint32_t currTick )
{
  set_state(STATE_STOP);
  app_dbg_msg("Time is up \n ");
}
*/
void check_water()
{
  if (pwater == true )
  {
    dio_turnon(OUTWATER) ;
    app_dbg_msg("Water problem \n ");
  }
  else {
    dio_turnoff(OUTWATER);
  }
}

void check_motor()
{
  if (pmotor == true)
  {
    dio_turnon(OUTMOTOR) ;
    app_dbg_msg("MOTOR problem \n ");
  }
  else {
    dio_turnoff(OUTMOTOR);

  }
}

void check_elec()
{
  if ( pelec == true)
  {
    dio_turnon(OUTELEC) ;
    app_dbg_msg("Elec problem \n ");
  }
  else
  {
    dio_turnoff(OUTELEC);
  }
}

void check_pos()
{
  if (ppos == true )
  {
    dio_turnon(OUTPOS) ;
    app_dbg_msg("obstacle\n ");
  }
  else
  {
    dio_turnoff(OUTPOS);
  }
}

void gethum (void)
{
  uint16_t Dhum ;

  /* Read humidity*/
  adc_controller_readadc(HUM, &Dhum);
  hum = Dhum * 100 / 4095 ;
}

void gettemp(void)
{
  uint16_t Dtemp ;

  /* Read temperature*/
  adc_controller_readadc(TEMP, &Dtemp);
  temp = (Dtemp * TMAX / 4095) + TMIN ;
}

void pivot21_init(void)
{
  set_state(STATE_INIT);
  curr_time = 0;
  start_time = 0;
}

void pivot21_task(void)
{
  gettemp() ;
  gethum() ;

  /*app_dbg_msg("Read temperature and humidity \n ");*/

  /* read state of manual/auto button*/
  man_state = dio_read(MAN_M);

  /* Read sensors*/
  ppos   = dio_read(INPOS);
  pmotor = dio_read(INMOTOR);
  pelec  = dio_read(INELEC);
  pwater = dio_read(INWATER);
  eco    = dio_read(ECO) ;
  
  curr_hr = curr_clock.time.hours;  /* put variable curr_hr equal to the actual hour (ask Abdallah)*/
  app_dbg_msg("time is %d \n", curr_hr );

  if (eco == true) 
  { 
    app_dbg_msg("I am in eco mode\n");
  }
  else 
  {
    app_dbg_msg("I am in normal mode\n");   
  }
  
  app_dbg_msg("Check problems \n ");
  /* Read current clock from RTC*/
  app_dbg_msg("reading current time \n");
  curr_time =  rtc_controller_gettime(); 
  app_dbg_msg("current_time is %d\n",curr_time);

  /* time */
  app_dbg_msg("I am checking the time interval\n");
  time_cond = curr_time - start_time; // time interval
  app_dbg_msg("time condtion is %d\n",time_cond);
  rtc_controller_getclock(&curr_clock); 
  
  /* curr_time += 500; time increments with 500ms*/
  /*if (eco == true)
  {
    app_dbg_msg(" ECO  TEST IF true\n");
    set_state( STATE_ECO );  enter the economic mode
  }
    */
  if (man_state == true ) /* manual mode or automatic mode*/
  {
    set_state(STATE_STOP);
  }

  if (check_failures()) /* indicating a problem*/
  {
    set_state(STATE_PROB);
  }
  else
  {
    dio_turnoff(OUTPOS);
    dio_turnoff(OUTWATER);
    dio_turnoff(OUTELEC) ;
    dio_turnoff(OUTMOTOR) ;
  }

  switch (state)
  {
  case STATE_INIT :
  {
    if (state_entery)
    {
      state_entery = false;
      app_dbg_msg("I am in initial mode \n");
    }
    /*dio_turnon(4);*/
    /*SYS_TMR_CallbackSingle (3000, 0, stp_timer_cb); to schedule  time operation*/
    /*dio_turnoff(5);*/
    set_state( STATE_NORMAL );

    break;
  }

  case STATE_NORMAL :
  {    
  
    if ((is_eco_time() && eco == true) || eco == false )
    {
 
      if (state_entery)
      {
        state_entery = false;
        app_dbg_msg("I am in normal mode\n");
      }

      if (init_cond)
      {
        init_cond = false;
        curr_time = start_time;
        time_cond = curr_time - start_time;
        app_dbg_msg("START TIME IS CURRENT TIME\n");      
      }

      if (is_weather_valid() && is_time_valid())
      {
        set_state(STATE_START); /* enter the start state to start the process of irrigation*/
      }
      else
      {
        set_state(STATE_STOP ); /* enter the stop state to stop the program */
      }
    }
    else 
    {
      set_state( STATE_STOP );  
    }

    break;
  }

  case STATE_STOP :
  {
    if (state_entery)
    {
      state_entery = false;
      app_dbg_msg("I am stopped\n");
    }

    SYS_TMR_CallbackStop ( time_move_handle );
    SYS_TMR_CallbackStop ( time_irrig_handle );
    dio_turnoff(MOTOR_FWD);
    dio_turnoff(MOTOR_BWD);
    dio_turnoff(WATER);  /* Turn off the pivot*/
    app_dbg_msg("I am stopped\n");

    if (man_state == true)
    {
      set_state(STATE_STOP); /* Enter the manual mode  */
    }
    else
    {
      set_state( STATE_INIT );
    }
    break;/* to stop or enter the manual mode*/
  }
  case STATE_START :
  {
    if (state_entery)
    {
      state_entery = false;
    }
    app_dbg_msg("I am starting to move\n");
    if ( read_start_time)
       {
        read_start_time = false ;
        app_dbg_msg("reading the start time \n");
        start_time = rtc_controller_gettime();
         app_dbg_msg("start_time is %d\n", start_time);
       }
         set_state(STATE_MOV );

  #if 0
     if(start_timer_mov == true)/*we can remove the true when the variable is bool*/
    {
      start_timer_mov = false;

      //SYS_TMR_CallbackSingle (time_on, 0, stp_timer_cb);
      /*to schedule  time operation*/
      app_dbg_msg("the timer is started\n");
    }
    #endif
  
    break;
    /* start movement and irrigation*/
  }
  case STATE_MOV :
  {
    if (state_entery)
    {
      state_entery = false;
      app_dbg_msg("I am moving\n");
      
      if (motor_direction == DIR_FWD)
        { 
        dio_turnoff(MOTOR_BWD);
        dio_turnon(MOTOR_FWD);
      }
      else
        {
        dio_turnoff(MOTOR_FWD);
        dio_turnon(MOTOR_BWD);
        }

     time_move_handle =  SYS_TMR_CallbackSingle (time_move * 1000, 0 , disp_done ); /*the pivot is done displacing*/
    }
    /*state = STATE_IRRIG ; <== removed*/
    /* we need to take in cosideration that angle it moves with, if we don't we would be working with only one stick!!*/
    
    if (!(( is_weather_valid() && is_time_valid()) && (( is_eco_time() && eco == true) || eco == false)))
    {    
      app_dbg_msg("stop time is up irrig \n ");
      set_state(STATE_STOP); /* enter the stop state to stop the program */
    }
    
  if (ppos)
    {
      if (new_ppos)
      {
        new_ppos = false;
        app_dbg_msg("Position problem detectected.\n");
        ppos_handle = SYS_TMR_CallbackSingle (5000, 0 , ppos_done ); /* Recheck for ppos after 5 seconds. */

      }
    }
     
    break;
    /* we add some breaks*/
    /* start movement and enter irrigation mode*/
  }

  case STATE_IRRIG :
  {
    if (state_entery)
    {
      state_entery = false;
      dio_turnon(WATER);
      app_dbg_msg("I am irigating\n");
      time_irrig_handle = SYS_TMR_CallbackSingle (time_irrig * 1000, 0 , irrig_done ); /*the pivot is done irrigating*/
    }

    if (!(( is_weather_valid() && is_time_valid()) && (( is_eco_time() && eco == true) || eco == false)))
    {    
      app_dbg_msg("stop time is up irrig \n ");
      set_state(STATE_STOP); /* enter the stop state to stop the program */
    }
    break;

    /* start irrigation*/
  }
  case STATE_PROB :
  {
    if (state_entery)
    {
      state_entery = false;
      app_dbg_msg("I have a problem\n");
      check_pos() ; 
      check_elec() ;
      check_motor();
      check_water();
      dio_turnoff(MOTOR_FWD);
      dio_turnoff(MOTOR_BWD);
      dio_turnoff(WATER);
      SYS_TMR_CallbackStop ( time_move_handle );
      SYS_TMR_CallbackStop ( time_irrig_handle );
      /* turn on the right LED that indicates the type of error*/
      set_state(STATE_MEH);
    }
    break;
    /* stop because of issues */
  }

  default :
  {
    app_dbg_msg("I am in default\n");
    break;
  }
  }
}


#if 0
 case STATE_ECO :
  { 
    app_dbg_msg(" ECO 1\n");
    if (state_entery)
    {
      state_entery = false;
    app_dbg_msg("I am in eco mode\n");
    }
    app_dbg_msg(" ECO 2\n");
    int curr_hr = curr_clock.time.hours;  /* put variable curr_hr equal to the actual hour (ask Abdallah) */
    app_dbg_msg("time is %d \n", curr_hr );
    
    if (curr_hr >= 22 || curr_hr <= 5)
    {
      set_state(STATE_NORMAL );  /* enter the normal mode */
    }
    else
    {
      set_state(STATE_STOP); /* enter the stop state to stop the program */
    }
    break;
  }
 #endif