#include <stdio.h>
#include <string.h>

#include "iec_std_lib.h"

#include "gpio.h"
#include "debug.h"
#include "app.h"

/**
 * Create Variables from List of generated variables
 */

/*

I - inputs
Q - outputs
M - memory

sizes 

X   Bit   1   BOOL
B   Byte  8   BYTE, SINT, USINT
W   Word  16  WORD, INT, UINT
D   Double word   32  DWORD, DINT, UDINT, FLOAT
L   Long word   64  LWORD, LINT, ULINT, DOUBLE


Python script, parse LOCATED_VAriaBLEs

generate 
*/

#define __LOCATED_VAR(type, name, ...) IEC_##type __##name;
#include "../srciec/LOCATED_VARIABLES.h"
#undef __LOCATED_VAR
#define __LOCATED_VAR(type, name, ...) IEC_##type* name = &__##name;
#include "../srciec/LOCATED_VARIABLES.h"
#undef __LOCATED_VAR

extern int common_ticktime__;
static int common_ticktime__ms;
static int tick = 0;
static long int curr_time_ms = 0;


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


/* Auto generated depending on the LOCATED_VARIABLES file */

#define MAX_INPUTS    1
#define MAX_OUTPUT    1
#define MAX_MEM_BYTE  1
#define MAX_MEM_WORD  1
#define MAX_MEM_DWORD 1
#define MAX_MEM_LONG  1
#define MAX_MEM_FLOAT  1

typedef struct
{
  uint8_t addr;
  bool state;
  int num;
}io_t;

static io_t hw_inputs[MAX_INPUTS];
static io_t hw_outputs[MAX_OUTPUT];
static uint8_t hw_mem_byte[MAX_MEM_BYTE];
static uint16_t hw_mem_word[MAX_MEM_WORD];
static uint32_t hw_mem_dword[MAX_MEM_DWORD];
static uint32_t hw_mem_float[MAX_MEM_FLOAT];
static uint64_t hw_mem_long[MAX_MEM_LONG];

/* Auto generated */
static void hw_init(void)
{
  hw_inputs[0].addr = 0;
  hw_inputs[0].state = 0;
  hw_inputs[0].num = 15;

  hw_outputs[0].addr = 0;
  hw_outputs[0].state = 0;
  hw_outputs[0].num = 2;

  hw_mem_float[0] = 0;
}


static void read_hw(void)
{
  int i;

  for (i = 0; i < MAX_INPUTS; i++)
  {
    if (hw_inputs[i].addr == 0)
    {
      /* from controller */
      hw_inputs[i].state = gpio_state_get(hw_inputs[i].num);
    }
    else
    {
      /* from extension */
      hw_inputs[i].state = gpio_state_get(hw_inputs[i].num);
    }

  }
}

static void write_hw(void)
{
  int i;

  for (i = 0; i < MAX_OUTPUT; i++)
  {
    if (hw_outputs[i].addr == 0)
    {
      /* from controller */
      gpio_state_write(hw_outputs[i].num, hw_outputs[i].state);
    }
    else
    {
      /* from extension */
      gpio_state_write(hw_outputs[i].num, hw_outputs[i].state);
    }

  }
}


void plc_updatetime(void)
{
  long int curr_time_ns;
  long int curr_time_s;

  curr_time_ms+= common_ticktime__ms;
  
  curr_time_s = curr_time_ms / 1000U;
  curr_time_ns = (curr_time_ms - curr_time_s * 1000U) * 1000000U;

  __CURRENT_TIME.tv_nsec = curr_time_ns;
  __CURRENT_TIME.tv_sec  = curr_time_s;
}

long int plc_init(void)
{
  common_ticktime__ms = common_ticktime__/1000000;
  debug_print("PLC Init. common tick : %d ms\n", common_ticktime__ms);
  config_init__();

  return common_ticktime__ms;
}


void plc_run(void)
{
  /* Read all physical inputs and memory inputs */
  read_hw();

  /* call the program_body */
  config_run__(tick++);

  /* perform all output actions and memory writes */
  write_hw();
}

void plc_set_mem0(float value)
{
  hw_mem_float[0] = value;
}

void get_current_time(void)
{
  debug_print("PLC Current time %ld ns %ld s\n", __CURRENT_TIME.tv_nsec, __CURRENT_TIME.tv_sec);
}