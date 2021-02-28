#include "unity.h"
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>

#include "xc_stub.h"
#include "gpio.h"

/* mock the hardware registers defined in the xc_stub.c */
extern volatile uint32_t TRISB;
extern volatile uint32_t LATB;
extern volatile uint32_t PORTB;

void setUp(void)
{
  gpio_init();
}

void tearDown(void)
{
}

/**
 * at the start the GPIO has an initiale state, defined in config file
 * this state can then be changed later.
 * here we test that after the init, the state is as it is defined.
 */
void test_gpio_is_input_or_ouput_after_init(void)
{

  #if (GPIO_RB0 == PIC32_GPIO_INPUT)
    TEST_ASSERT_TRUE(gpio_isinput(pinB0));
  #else
    TEST_ASSERT_TRUE(gpio_isoutput(pinB0));
  #endif
  
  #if (GPIO_RB12 == PIC32_GPIO_OUTPUT)
    TEST_ASSERT_TRUE(gpio_isoutput(pinB12));
  #else
    TEST_ASSERT_TRUE(gpio_isinput(pinB12));
  #endif
}

/** 
 * once the direction is set in the init, it can changed in runtime
 */
void test_gpio_set_b12_opposite_direction_then_read_it(void)
{
  #if (GPIO_RB12 == PIC32_GPIO_OUTPUT)
    gpio_input_set(pinB12);
    TEST_ASSERT_TRUE(gpio_isinput(pinB12));
  #else
    gpio_output_set(pinB12);
    TEST_ASSERT_TRUE(gpio_isoutput(pinB12));
  #endif
}


void test_gpio_get_state_of_output_at_init_is_clear(void)
{
  TEST_ASSERT_FALSE(gpio_state_get(pinB12));
}

/**
 * changing the state in PIC32 is done by setting the LAT reg
 * reading the state of gpio is done by reading the PORT reg.
 * since the reg are mocked as global variables the tests affects each other
 * we must preserve their value before using then restore it.
 */
void test_gpio_set_state_of_output_then_get_state(void)
{

  uint32_t old_value_portb;
  uint32_t old_value_latb;

  TEST_ASSERT_FALSE(gpio_state_get(pinB12));

  old_value_latb = LATB;
  gpio_state_set(pinB12, true);
  /* MCU write to lat and reads from port, in the hardware they are equals
   * we set them equals manually in test.*/
  old_value_portb = PORTB;
  PORTB = LATB;
  TEST_ASSERT_TRUE(gpio_state_get(pinB12));

  /* restore the portb to not affect the other tests, because these are defined in support*/
  PORTB = old_value_portb;
  LATB = old_value_latb;
}

void test_gpio_toggle_output_changes_its_state(void)
{
  uint32_t old_value_portb;
  uint32_t old_value_latb;

  TEST_ASSERT_FALSE(gpio_state_get(pinB12));
  old_value_latb = LATB;

  gpio_state_toggle(pinB12);
  
  old_value_portb = PORTB;
  PORTB = LATB;
  
  TEST_ASSERT_TRUE(gpio_state_get(pinB12));

  /* restore lat and port */
  PORTB = old_value_portb;
  LATB = old_value_latb;
}