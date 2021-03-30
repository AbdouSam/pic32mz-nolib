#ifndef INCLUDED_PIC32_UART_H
#define INCLUDED_PIC32_UART_H

#include <stdint.h>

#include "interrupt.h"

typedef enum
{
  PIC32_UART_1,
  PIC32_UART_2,
  PIC32_UART_3,
  PIC32_UART_4,
  PIC32_UART_5,
  PIC32_UART_6,
  PIC32_UART_MAX,
}pic32_uart_t;

typedef enum
{
  NO_PARITY_8_BIT_DATA      = (0 << 1),
  EVEN_PARITY_8_BIT_DATA    = (1 << 1),
  ODD_PARITY_8_BIT_DATA     = (2 << 1),
  NO_PARITY_9_BIT_DATA      = (3 << 1),
}pic32_uart_parity_data_t;

typedef enum
{
  ONE_STOP_BIT  = 0,
  TWO_STOP_BITS = 1,
}pic32_uart_stop_bit_t;

typedef enum
{
  IF_RBUF_NOT_EMPTY         = (0 << 6),
  IF_RBUF_FULL_1_2_OR_MORE  = (1 << 6),
  IF_RBUF_FULL_3_4_OR_MORE  = (2 << 6),
  IF_RBUF_RESERVED          = (3 << 6),
}pic32_uart_rxi_t;

typedef enum
{
  IF_TBUF_AT_LEAST_ONE      = (0 << 14),
  IF_TBUF_ALL_TRANSMITED    = (1 << 14),
  IF_TBUF_IS_EMPTY          = (2 << 14),
  IF_TBUF_RESERVED          = (3 << 14),
}pic32_uart_txi_t;

int uart_rx_any(pic32_uart_t uart_id);

int uart_rx_char(pic32_uart_t uart_id);

void uart_tx_char(pic32_uart_t uart_id, int c);

int uart_init(pic32_uart_t uart_id,
               pic32_uart_parity_data_t pdata,
               pic32_uart_stop_bit_t stop_bit,
               uint32_t baud);

/**
 * @brief set uart receive interrupt
 *
 * @param uart_id the uart id
 * @param subp subpriority, priority being set at compile time (see README)
 * @param imode interrupt mode, trigger interrupt if one character is received
 *  or 3, ..etc
 * @param intcb the callback of interrupt void f(void)
 */

int uart_rxi_set(pic32_uart_t uart_id, 
                 uint8_t subp, 
                 pic32_uart_rxi_t imode,
                 int_callback_ft intcb);

#endif /* INCLUDED_PIC32_UART_H */