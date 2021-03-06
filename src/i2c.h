#ifndef INCLUDED_PIC32_I2C_H
#define INCLUDED_PIC32_I2C_H

void i2c_init(double speed);

void i2c_write(unsigned char address, char wait_ack);

void i2c_read(unsigned char * value, char ack_nack);

void i2c_restart(void);

void i2c_stop(void);

void i2c_start(void);

#endif /* INCLUDED_PIC32_I2C_H */