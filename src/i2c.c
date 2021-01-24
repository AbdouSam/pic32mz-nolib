#include <xc.h>
#include "sysclk.h"

#define I2C_TPGD (104 * (10^(-9)))

/* I2C uses 7bit slave address, the the 8th bit sets the mode
 * the reason of this connection, Write mode (8th : 0) or Read mode (8th: 1)
 * The last bit is addes as the last bit in the address, which is the first thing to send
 * after the start bit (the start bit is generated automatically.)
 * if address is 0x68, we always shift to the left 0x68 << 1 = 0xD0
 * then if we want to write data we leave it as it is (0)
 * if we want to Read data we set the bit to 1 0xD0 | 1 = 0xD1
 *
 * if the device is connected and ready we must receice an ACK signal in the 9th bit.
 * or the 9th clock cycle from the begining.
 *
 * Then immediatly after receiving the ACK we send in the Regitster  address
 * we want to read from or write to, the devices responds by an ACK if the address is valid
 *
 * Then after the ACK is received we have a couple of options
 * - if we want to write a single byte of data we send The data directly receive another ACk
 *   then stop with a stop contion
 * - if we want to read the register we set we send again a start conditoion with read bit (RW)
 *   we receive the ack again, ,then read the data directly afterward
 * - if we want to read a sequence of bytes. we continue reading until we are done then we stop
 *   with a nack then a stop condition
 * - if we want to write a sequence, we keep sending data after each Ack until we send a stop
 *   condition.
 *  
 * we proceed to Read the Data itself or Write the data.
 * the Data format is 8 bits at a time.
 *
 * After the data received if it is a read, we don't need to wait for Ack, the data is a response
 * that the device is working, we continue to read as much as we wont, we send an NACK to the device
 * to inform the device to stop sending data
 * or After a write we just stop writing
 * Send a stop bit.
 *
 * if want to re write or re-read again we have to start from the begining by start bit address etc...
 */
void i2c_init(double speed)
{

  // the i2C used in the board is SDA2 -> RA3, SCL2 -> RA2
  // Setting up speed formula ((1 / (2 * Fsck) - Tpgd) * PBCLK)
  // Tpgd : propagation delay set as 104ns
  // Fsck the speed we want 100kHz for example
  // PBCLK pheripheral clock speed for I2C

  double bgr_reg;
  
  I2C2CON = 0; // disable before setting
  I2C2CONbits.DISSLW = 1; // disable slwe rate control for low speeds like 100 kHz

  bgr_reg = (1 / (2 * speed)) - I2C_TPGD;
  bgr_reg *=(sysclk_uartfreq_get()) - 2;

  I2C2BRG = (int) bgr_reg;
  I2C2CONbits.ON = 1;
}

void i2c_wait_for_idle(void)
{
  /* There is noAcknowlege sequence in progress, no Receiving sequence
   * no stop condition nor Repeated Start conditions are in progress
   * no Start condition in progress, the bus is idle and ready to start 
   */
  while (I2C2CON & 0x1F);

  /* 
   * trstat bit = 0, No transmit from master is in progress.
   */

  while (I2C2STATbits.TRSTAT); 
}

void i2c_start(void)
{
  /* send a start condition */
  i2c_wait_for_idle();
  I2C2CONbits.SEN = 1;

  while (I2C2CONbits.SEN == 1); // start condition in progress
}

void i2c_stop(void)
{
  /* send a stop condition */
  i2c_wait_for_idle();
  I2C2CONbits.PEN = 1;
}


void i2c_restart(void)
{
  /* Send a repeated start condition*/
  i2c_wait_for_idle();
  I2C2CONbits.RSEN = 1;
  while(I2C2CONbits.RSEN == 1); // wait for it to finish.
}


void i2c_ack(void)
{
  i2c_wait_for_idle();
  I2C2CONbits.ACKDT = 0; // set to ack, = 1 to send NACK
  I2C2CONbits.ACKEN = 1; // send the ACK bit
  while (I2C2CONbits.ACKEN); // sending ack finished.
}

void i2c_nack(void)
{
  i2c_wait_for_idle();
  I2C2CONbits.ACKDT = 1; // set to ack, = 1 to send NACK
  I2C2CONbits.ACKEN = 1; // send the ACK bit
  while (I2C2CONbits.ACKEN); // sending ack finished.
}

/* write data to address.
 * note that the address should already by shifted here
 * we don't shift in here, because we want to use i2c_write for various
 * scenario, like write only, or write and read.
 * wait_ack: to wait for an ack after writing the data, or not.
 */
void i2c_write(unsigned char address, char wait_ack)
{
  I2C2TRN = address; // RW=0 to write
  while(I2C2STATbits.TBF == 1); // transmit buffer empty, tr complete

  i2c_wait_for_idle(); // wait until idle after write
  if (wait_ack)
  {
    while (I2C2STATbits.ACKSTAT == 1); // wait until ack is received.
  }
}

void i2c_read(unsigned char *value, char ack_nack)
{
  I2C2CONbits.RCEN = 1; // receive enable, the bits clears after receiving 8 bits

  while(I2C2CONbits.RCEN); // receive complete
  while (!I2C2STATbits.RBF); // read buffer is full, when 8 bits are read
  //the bit clear automatically after reading the reg:

  *value = I2C2RCV;

  if (!ack_nack)
  {
    i2c_ack(); // send ACK to continue reading
  }
  else
  {
    i2c_nack(); // send NACK to stop reading.
  }

}