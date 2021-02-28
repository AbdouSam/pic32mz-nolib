/*
 * Test run the MCP79411 with i2C
 */
#include "i2c.h"
#include "rtc.h"
#define RTCSRAM_ADDR    0xDE
#define REG_SEC         (0x00)

void rtc_write(unsigned char reg_addr, unsigned char value)
{
  i2c_start();
  i2c_write(RTCSRAM_ADDR, 1);
  i2c_write(reg_addr, 1);
  i2c_write(value, 1);
  i2c_stop();
}

void rtc_read(unsigned char reg_addr, unsigned char *value)
{
  i2c_start();
  i2c_write(RTCSRAM_ADDR, 1);
  i2c_write(reg_addr, 1);
  i2c_restart();
  i2c_write(RTCSRAM_ADDR | 1, 1);
  i2c_read(value, 1);
  i2c_stop();
}

void rtc_read_time(unsigned char *value)
{
  i2c_start();
  i2c_write(RTCSRAM_ADDR, 1);
  i2c_write(REG_SEC, 1);
  i2c_restart();
  i2c_write(RTCSRAM_ADDR | 1, 1);
  i2c_read(&value[0], 0);   /* sec */
  i2c_read(&value[1], 1);   /* min */
  i2c_stop();
}

/*
 * Read previous value of Seconds, or it with the OSCillator enable
 * write it back right away. it enabled
 */

void rtc_init(void)
{
  unsigned char value;

  rtc_read(REG_SEC, &value);
  rtc_write(REG_SEC, value | 0x80); /* Set OSCILATOR bit */
}