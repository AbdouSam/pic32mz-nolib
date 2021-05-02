#include "pivot21.h"
#include "dio.h"
#include "adc_controller.h"
#include "system/tmr/sys_tmr.h"
#include "rtc_controller.h"
#include "debug.h"

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
#define STATE_STOP   1 /* to stop the program*/
#define STATE_MOV    2 /* to start moving the pivot*/
#define STATE_IRRIG  3 /* to start watering the field*/
#define STATE_ECO    4 /* to use the economic mode */
#define STATE_NORMAL 5 /* tu use the normal mode*/
#define STATE_PROB   6 /* to indicates some issues and stop the program*/
#define STATE_START  7 /* to manage between irrigation and movement*/
#define STATE_MAN    8 /* to enter the manual mode*/
#define STATE_MEH   (19) /* to enter the manual mode*/

#define TMAX         (80)  /* reading the max value of the sensor*/
#define TMIN         (-20)   /* reading the min value OF THE SENSOR*/
#define MINUTE       (60000) /* csnt for one minute */

/* Digital Outputs*/
#define MOTOR       0   /* signal to turn on the motor*/
#define WATER       1   /* signal to the pompe */
#define OUTELEC     2   /* LED that indicates problem in electricity */
#define OUTMOTOR    3   /* LED that indicates problem in the motor*/
#define OUTWATER    4   /* LED that indicates lack in the water*/
#define OUTPOS      5   /* LED that indicates an obstacle*/
/* Analog Inputs*/
#define TEMP        0     /* reading from temperature sensure */
#define HUM         1     /* reading from humidity sensure*/


/* add 2 more relays for marche avant , arriere*/
/* we make sure we switch one off to open the other*/


/* variable set*/

static int state;
static uint16_t temp;
static uint16_t hum;
static bool ppos;
static bool pelec;
static bool pmotor;
static bool pwater;
static bool eco ;
static bool start_timer_mov = true ; /* this should be connected to a bool pin */
static rtc_clock curr_clock;
static bool state_entery = true ;
//static uint32_t time_on = 20;
static uint32_t time_irrig = 5000;
static uint32_t time_move = 5000;
static uint32_t start_time;
static uint32_t curr_time;
static bool read_start_time = true ;
static int time_cond;
static bool init_cond = true;
static SYS_TMR_HANDLE time_move_handle;
static SYS_TMR_HANDLE time_irrig_handle;



void set_state (int st)
{
  state = st;
  state_entery = true;
}

void irrig_done ( uintptr_t context, uint32_t currTick )
{
      if (((hum < 50 && temp < 27 ) || (hum < 20 && temp > 40 ) ) && (time_cond <= 20) )
    {
         
        set_state(STATE_MOV);
        app_dbg_msg("I am done irrigating! \n ");
    }
    else {
      app_dbg_msg("stop time is up irrig \n ");
      set_state(STATE_STOP ); /* enter the stop state to stop the program */
    }

}
void disp_done ( uintptr_t context, uint32_t currTick )
{ 
    if (((hum < 50 && temp < 27 ) || (hum < 20 && temp > 40 ) ) && (time_cond <= 20) )
    {

  set_state(STATE_IRRIG);
  app_dbg_msg("I am done moving! \n ");
    }
    else {
       app_dbg_msg("stop time is up mooove \n ");
      set_state(STATE_STOP ); /* enter the stop state to stop the program */
    }


}

void stp_timer_cb( uintptr_t context, uint32_t currTick )
{
  set_state(STATE_STOP);
  app_dbg_msg("Time is up \n ");
}

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
  bool man_state = dio_read(MAN_M);

  /* Read sensors*/
  ppos   = dio_read(INPOS);
  pmotor = dio_read(INMOTOR);
  pelec  = dio_read(INELEC);
  pwater = dio_read(INWATER);
  eco    = dio_read(ECO) ;

  app_dbg_msg("Check problems \n ");
  /* Read current clock from RTC*/
  app_dbg_msg("reading current time \n");
  curr_time =  rtc_controller_gettime(); 
  app_dbg_msg("current_time is %d",curr_time);

    /* time */
    app_dbg_msg("I am checking the time interval\n");
    time_cond = curr_time - start_time; // time interval
     app_dbg_msg("time condtion is %d",time_cond);
    rtc_controller_getclock(&curr_clock); 
 /* curr_time += 500; time increments with 500ms*/
    if (eco == true)
    {
      app_dbg_msg(" ECO  TEST IF true\n");
      set_state( STATE_ECO ); /* enter the economic mode*/
    }
    if (man_state == true ) /* manual mode or automatic mode*/
    {
      set_state(STATE_STOP);
    }
    if (ppos == true || pmotor == true || pwater == true || pelec == true) /* indicating a problem*/
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

    if (eco == false )
    {
      set_state( STATE_NORMAL );
    }
    /* enter the normal mode      */
    break;
  }
  case STATE_ECO :
  { 
    app_dbg_msg(" ECO 1\n");
    if (state_entery)
    {
      state_entery = false;
    app_dbg_msg("I am in eco mode\n");
    }
    app_dbg_msg(" ECO 2\n");
    int hr = curr_clock.time.hours;  /* put variable hr equal to the actual hour (ask Abdallah)*/
    app_dbg_msg("time is %d \n", hr );
    
    if (hr >= 22 || hr <= 5)
    {
      set_state(STATE_NORMAL );  /* enter the normal mode*/
    }
    else
    {
      set_state(STATE_STOP); /* enter the stop state to stop the program */
    }
    break;
  }

  case STATE_NORMAL :
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
    if (((hum < 50 && temp < 27 ) || (hum < 20 && temp > 40 )) && (time_cond <= 20))
    {

      set_state(STATE_START); /* enter the start state to start the process of irrigation*/
    }
    else {
      set_state(STATE_STOP ); /* enter the stop state to stop the program */
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
    dio_turnoff(MOTOR);
    dio_turnoff(WATER);  /* Turn off the pivot*/
    app_dbg_msg("I am stopped\n");
    init_cond = true;
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
         app_dbg_msg("start_time is %d", start_time);
       }

    if (start_timer_mov == true)/*we can remove the true when the variable is bool*/
    {
      start_timer_mov = false;

      //SYS_TMR_CallbackSingle (time_on, 0, stp_timer_cb);
      /*to schedule  time operation*/
      app_dbg_msg("the timer is started\n");
      set_state(STATE_MOV );
    }
  
    break;
    /* start movement and irrigation*/
  }
  case STATE_MOV :
  {
    if (state_entery)
    {
      state_entery = false;
      app_dbg_msg("I am moving\n");
      dio_turnon(MOTOR);
     time_move_handle =  SYS_TMR_CallbackSingle (time_move, 0 , disp_done ); /*the pivot is done displacing*/
    }
    /*state = STATE_IRRIG ; <== removed*/
    /* we need to take in cosideration that angle it moves with, if we don't we would be working with only one stick!!*/
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
      time_irrig_handle = SYS_TMR_CallbackSingle (time_irrig, 0 , irrig_done ); /*the pivot is done irrigating*/
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
      check_pos()  ;
      check_elec() ;
      check_motor();
      check_water();
      dio_turnoff(MOTOR);
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

