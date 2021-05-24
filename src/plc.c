#include "iec_std_lib.h"
#include <stdio.h>
#include <string.h>
#include "gpio.h"
#include "debug.h"
#include "app.h"



/**
 * Functions and variables to export to generated C softPLC
 * The next macros create Variables with the same name as the addresses
 * set in PLC project
 */

#define __LOCATED_VAR(type, name, ...) type __##name;
#include "../srciec/LOCATED_VARIABLES.h"
#undef __LOCATED_VAR
#define __LOCATED_VAR(type, name, ...) type* name = &__##name;
#include "../srciec/LOCATED_VARIABLES.h"
#undef __LOCATED_VAR

extern int common_ticktime__;
static int common_ticktime__ms;
static int tick = 0;
static uint64_t curr_time_ms = 0;

/* __CURRENT_TIME is Read by the program_body, and serves as the base time
 * it has 2 parameters, the seconds, and the nanoseconds, meaning
 * 3.8s is 3seconds and 800,000,000ns
 * it must be updated as often as possible.
 */

IEC_TIME __CURRENT_TIME;

/**
 * Functions and variables provied by generated C softPLC
 */ 

void config_run__(int tick);
void config_init__(void);


long int plc_init(void)
{
  common_ticktime__ms = common_ticktime__/1000000;
  debug_print("PLC Init. common tick : %d ms\n", common_ticktime__ms);
  config_init__();

  return common_ticktime__ms;
}

void plc_run(void)
{
  /** update the current tick for body
   */
  curr_time_ms+= common_ticktime__ms;

  __CURRENT_TIME.tv_nsec = curr_time_ms*1000000;
  __CURRENT_TIME.tv_sec  = curr_time_ms/1000;

  /* Read all physical inputs and memory inputs */
  *__IX0_0 = gpio_state_get(DIG_IN1);

  /* call the program_body */
  config_run__(tick++);

  /* perform all output actions and memory writes */
  gpio_state_write(RELAY_OUT5, *__QX0_1);
}

void get_current_time(void)
{
  debug_print("PLC Current time %ld ns %ld s\n", __CURRENT_TIME.tv_nsec, __CURRENT_TIME.tv_sec);
}