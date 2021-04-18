
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <stdint.h>

/* Config includes. */

#include "config.h"
#include "debug.h"
#include "error.h"

#include "adc_controller.h"
#include "bsp.h"
#include "system/tmr/sys_tmr.h"
#include "dio.h"
#include "rtc_controller.h"

#include "pivot_pfe.h"

#define HUM_MAX_RANGE                100
#define HUM_MIN_RANGE                0

#define TEM_MAX_RANGE                50
#define TEM_MIN_RANGE                0

#define HUMIDITY_MIN                 50
#define HUMIDITY_MAX                 80
#define TEMPERATURE_MAX              27

#define MOTOR_DEFAULT_WORKTIME       6
#define PUMP_DEFAULT_WORKTIME        54

#define ANALOG_INPUT_TEMP            ADC_IN1
#define ANALOG_INPUT_HUMI            ADC_IN2

#define DI_INPUT_DEFAUT_ALIM         BSP_INPUT_1
#define DI_INPUT_DEFAULT_PHASE       BSP_INPUT_2
#define DI_INPUT_DEFAULT_PUMP_PRES   BSP_INPUT_3
#define DI_INPUT_DEFAULT_PUMP_THERM  BSP_INPUT_4
#define DI_INPUT_DEFAUT_MOTEUR       BSP_INPUT_5
#define DI_INPUT_FC_OBSTACLE         BSP_INPUT_6
#define DI_INPUT_FC_FIN_CYCLE        BSP_INPUT_7
#define DI_INPUT_MODE                BSP_INPUT_8

#define DI_OUT_PUMP_WAT              BSP_OUTPUT_15
#define DI_OUT_MOTOR                 BSP_OUTPUT_16

#define START_ECONOMIQUE_TIME        22*60
#define END_ECONOMIQUE_TIME          5*60

#define HUMIDITY_MIN_VALUE    20 /* °H */
#define TEMPERATURE_MIN_VALUE 40 /* °C */

typedef enum state_m
{
  STATE_CONFIG = 0,
  STATE_INIT,
  STATE_DEFAUT,
  STATE_PIVOT_MOVE,
  STATE_ARROSAGE,
  STATE_CYCLE_END,
  STATE_MANUAL,
  STATE_CHECK_WEATHER,
}state_m_t;

typedef enum system_mode
{
  SYSTEM_DEACTIVATE,
  SYSTEM_MODE_AUTO,
  SYSTEM_MODE_ECO,
}system_mode_t;

typedef enum pvt_req
{
  PVTPFE_REQUEST_HUMI_TEMP,
  PVTPFE_REQUEST_STATE,
  PVTPFE_REQUEST_CYCLE_NUMBER,
  PVTPFE_REQUEST_MODE
}pvt_req_t;


static uint8_t cycle_number;
static uint8_t counter;

static SYS_TMR_HANDLE pvtpfe_handle;
static double temperature;
static double humidity;

static state_m_t state;
static system_mode_t system_mode;

static uint16_t motor_worktime;
static uint16_t arrosage_time;

static uint16_t adc_humidity_raw    = 0;
static uint16_t adc_temperature_raw = 0;

static rtc_clock local_time;

void get_config_data(uint16_t motor_time, uint16_t pump_time, char *retframe)
{
  app_dbg_msg("pivot pfe : get_config_data\n");

  if (motor_time > 2 && pump_time > 15)
  {
    motor_worktime = motor_time;
    arrosage_time  = pump_time;

    app_dbg_msg("pivot pfe : accepted ! motor : %03d pump : %03d\n", motor_worktime, arrosage_time);
    sprintf(retframe, "@00NcOK");
  }
  else
  {
    app_dbg_msg("pivot pfe : refused ! motor : %03d pump : %03d\n", motor_worktime, arrosage_time);
    sprintf(retframe, "@00NcERROR");
  }
}

static bool economique_time(void)
{
  uint16_t time_minutes = 0;

  rtc_controller_getclock(&local_time);

  time_minutes = local_time.time.hours*60 + local_time.time.minutes;

  if (time_minutes >= START_ECONOMIQUE_TIME || time_minutes <= END_ECONOMIQUE_TIME)
    {
      return true;
    }

  return false;
}


static void motor_on(void)
{
  bsp_pin_set(DI_OUT_MOTOR);
}

static void motor_off(void)
{
  bsp_pin_clear(DI_OUT_MOTOR);
}

static void pump_on(void)
{
  bsp_pin_set(DI_OUT_PUMP_WAT);
}

static void pump_off(void)
{
  bsp_pin_clear(DI_OUT_PUMP_WAT);
}

static bool manual_mode(void)
{
  if (bsp_pin_get(DI_INPUT_MODE))
  {
    return true;
  }

  return false;
}

// check hardware
static bool fin_de_course(void)
{
  return bsp_pin_get(DI_INPUT_FC_FIN_CYCLE);
}

static double read_humidity(void)
{
  adc_controller_readadc(ANALOG_INPUT_HUMI, &adc_humidity_raw);

  return (((double)((HUM_MAX_RANGE-HUM_MIN_RANGE)/4095.0))*(adc_humidity_raw-4095) + HUM_MAX_RANGE);
}

static double read_temperature(void)
{
  adc_controller_readadc(ANALOG_INPUT_TEMP, &adc_temperature_raw);

  return ((((TEM_MAX_RANGE-TEM_MIN_RANGE)/4095.0))*(adc_temperature_raw-4095) + TEM_MAX_RANGE);
}

static void pvtpfe_callback(uintptr_t context, uint32_t currTick)
{

  humidity    = read_humidity();
  temperature = read_temperature();

  app_dbg_msg("Pivot : H = %0.2f %, T = %0.2f C\n", humidity, temperature);

  counter = counter + 1;
}

void pivot_pfe_request(int req, char *retframe)
{
  switch(req)
  {
    case PVTPFE_REQUEST_HUMI_TEMP:
      {
        sprintf(retframe, "@00NrH%03XT%03X", adc_humidity_raw, adc_temperature_raw);
      }
      break;

    case PVTPFE_REQUEST_STATE:
      {
        sprintf(retframe, "@00Nr%01u", state);
      }
      break;

    case PVTPFE_REQUEST_CYCLE_NUMBER:
      {
        sprintf(retframe, "@00Nr%03u", cycle_number);
      }
      break;

    case PVTPFE_REQUEST_MODE:
      {
        sprintf(retframe, "@00Nr%01u", system_mode);
      }
      break;

    default:
      {
        sprintf(retframe, "@00NrERROR");
      }
      break;
  }
}

void pivot_pfe_set(int mode, char *retframe)
{
  switch (mode)
  {
    case SYSTEM_DEACTIVATE:
      {
        system_mode = SYSTEM_DEACTIVATE;
        sprintf(retframe, "@00Ns%1u", system_mode);
      }
      break;

    case SYSTEM_MODE_AUTO:
      {
        system_mode = SYSTEM_MODE_AUTO;
        sprintf(retframe, "@00Ns%1u", system_mode);
      }
      break;

    case SYSTEM_MODE_ECO:
      {
        system_mode = SYSTEM_MODE_ECO;
        sprintf(retframe, "@00Ns%1u", system_mode);
      }
      break;

    default:
      {
        sprintf(retframe, "@00NaERROR");
      }
      break;
  }
}

void pivot_pfe_init(void)
{
  temperature    = 0;
  humidity       = 0;
  counter        = 0;
  cycle_number   = 0;

  system_mode    = SYSTEM_DEACTIVATE;

  motor_worktime = MOTOR_DEFAULT_WORKTIME;
  arrosage_time  = PUMP_DEFAULT_WORKTIME;

  state = STATE_CONFIG;
}

static bool read_humidity_D(void)
{
  return bsp_pin_get(BSP_INPUT_CAPTURE_1);
}

static bool read_temperature_D(void)
{
  return bsp_pin_get(BSP_INPUT_CAPTURE_2);
}



/*
  assumption :
  fault = 1
  non fault = 0
*/
static bool default_system(void)
{
  if (bsp_pin_get(DI_INPUT_DEFAUT_ALIM)         ||
      bsp_pin_get(DI_INPUT_DEFAULT_PHASE)       ||
      bsp_pin_get(DI_INPUT_DEFAULT_PUMP_PRES)   ||
      bsp_pin_get(DI_INPUT_DEFAULT_PUMP_THERM)  ||
      bsp_pin_get(DI_INPUT_DEFAUT_MOTEUR)       ||
      bsp_pin_get(DI_INPUT_FC_OBSTACLE)          )
    {
      return true;
    }

  return false;
}

void pivot_pfe_task(void)
{
  if (state > STATE_CONFIG)
  {

    if (default_system())
      {
        motor_off();
        pump_off();
        state = STATE_DEFAUT;
      }
    else if (manual_mode())
      {
        pump_off();
        motor_off();
        state = STATE_MANUAL;
      }
  }

  switch (state)
  {
    case STATE_CONFIG:
    {
      app_dbg_msg("pivot pfe: state STATE_CONFIG\n");

      pvtpfe_handle = SYS_TMR_CallbackPeriodic(1000, 1, pvtpfe_callback);

      if (pvtpfe_handle == SYS_TMR_HANDLE_INVALID)
        {
          app_dbg_msg("pivot pfe : Unable to get valid timer!\n");
        }
      else
        {
          state = STATE_INIT;
        }
    }
    break;

    case STATE_INIT:
    {
      app_dbg_msg("pivot pfe: state STATE_INIT\n");

      if (system_mode == SYSTEM_DEACTIVATE)
      {
        state = STATE_INIT;
      }

      if (system_mode == SYSTEM_MODE_AUTO)
      {
        state = STATE_CHECK_WEATHER;
      }

      if (system_mode == SYSTEM_MODE_ECO)
      {
        if (economique_time())
        {
          state = STATE_CHECK_WEATHER;
        }
      }
    }
    break;

    case STATE_CHECK_WEATHER:
    {
      app_dbg_msg("pivot pfe: state STATE_CHECK_WEATHER\n");



      if (system_mode == SYSTEM_DEACTIVATE)
      {
        state = STATE_INIT;
        break;
      }

      if (humidity <= HUMIDITY_MIN_VALUE)
      {
        counter = 0;
        motor_on();
        state = STATE_PIVOT_MOVE;
        break;
      }

      if (temperature >= TEMPERATURE_MIN_VALUE)
      {
        counter = 0;
        motor_on();
        state = STATE_PIVOT_MOVE;
        break;
      }
      //if ((humidity < HUMIDITY_MIN) && (temperature < TEMPERATURE_MAX)) // recheck later
      if (read_humidity_D() && read_temperature_D()) // recheck later
      {
        counter = 0;
        motor_on();
        state = STATE_PIVOT_MOVE;
      }
    }
    break;

    case STATE_PIVOT_MOVE:
    {
      app_dbg_msg("pivot pfe: state STATE_PIVOT_MOVE\n");

      if (system_mode == SYSTEM_DEACTIVATE)
      {
        state = STATE_INIT;
        motor_off();
        break;
      }

      if (fin_de_course())
        {
          state = STATE_CYCLE_END;
          break;
        }

      if (counter >= motor_worktime)
      {
        motor_off();
        counter = 0;
        pump_on();
        state = STATE_ARROSAGE;
      }
    }
    break;

    case STATE_ARROSAGE:
    {
      app_dbg_msg("pivot pfe: state STATE_ARROSAGE\n");

      if (system_mode == SYSTEM_DEACTIVATE)
      {
        state = STATE_INIT;
        pump_off();
        break;
      }

      if (counter >= arrosage_time)
      {
        pump_off();
        motor_on();
        counter = 0;
        state = STATE_PIVOT_MOVE;
      }
    }
    break;

    case STATE_CYCLE_END:
    {
      app_dbg_msg("pivot pfe: state STATE_CYCLE_END\n");

      cycle_number = cycle_number + 1;

      if (system_mode == SYSTEM_DEACTIVATE)
      {
        pump_off();
        motor_off();
        state = STATE_INIT;
        break;
      }

      pump_off();
      motor_off();

      state = STATE_INIT;
    }
    break;

    case STATE_MANUAL:
    {
      app_dbg_msg("pivot pfe: state STATE_MANUAL\n");

      if (system_mode != SYSTEM_DEACTIVATE && !manual_mode())
      {
        state = STATE_INIT;
      }
    }
    break;

    case STATE_DEFAUT:
    {
      app_dbg_msg("pivot pfe: state STATE_DEFAUT\n");

      if (system_mode == SYSTEM_DEACTIVATE)
      {
        state = STATE_MANUAL;
        break;
      }

      if (!default_system())
      {
        state = STATE_INIT;
        break;
      }

      if (manual_mode())
      {
        state = STATE_MANUAL;
        break;
      }

    }
    break;
  }
}