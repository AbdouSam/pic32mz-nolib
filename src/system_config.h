#ifndef _SYSTEM_CONFIG_H
#define _SYSTEM_CONFIG_H

#include "config.h"

/* Clock System Service Configuration Options */
#define SYS_CLK_FREQ                                200000000ul
#define SYS_CLK_BUS_PERIPHERAL_1                    100000000ul
#define SYS_CLK_BUS_PERIPHERAL_2                    100000000ul
#define SYS_CLK_BUS_PERIPHERAL_3                    40000000ul
#define SYS_CLK_BUS_PERIPHERAL_4                    100000000ul
#define SYS_CLK_BUS_PERIPHERAL_5                    100000000ul
#define SYS_CLK_BUS_PERIPHERAL_7                    200000000ul
#define SYS_CLK_BUS_PERIPHERAL_8                    100000000ul
#define SYS_CLK_CONFIG_PRIMARY_XTAL                 24000000ul
#define SYS_CLK_CONFIG_SECONDARY_XTAL               0ul

/* Interrupt System Service Configuration */
#define SYS_INT                                     true

/* BSP Ports Configuration */
#define SYS_PORT_A_ANSEL                            0xFFFF
#define SYS_PORT_A_TRIS_OUT                         0x6C3
#define SYS_PORT_A_TRIS_IN                          0xC000
#define SYS_PORT_A_LAT                              0x0
#define SYS_PORT_A_ODC                              0x0

#define SYS_PORT_B_ANSEL                            0xFFFF
#define SYS_PORT_B_TRIS_OUT                         0xB83C
#define SYS_PORT_B_LAT                              0x0
#define SYS_PORT_B_ODC                              0x0

#define SYS_PORT_C_ANSEL                            0xFFFF
#define SYS_PORT_C_TRIS_OUT                         0x4000
#define SYS_PORT_C_TRIS_IN                          0x2000
#define SYS_PORT_C_LAT                              0x0
#define SYS_PORT_C_ODC                              0x0

#define SYS_PORT_D_ANSEL                            0xFFFF
#define SYS_PORT_D_TRIS_IN                          0x84B1
#define SYS_PORT_D_TRIS_OUT                         0x02
#define SYS_PORT_D_LAT                              0x0
#define SYS_PORT_D_ODC                              0x0

#define SYS_PORT_E_ANSEL                            0xFFFF
#define SYS_PORT_E_TRIS_OUT                         0x300
#define SYS_PORT_E_LAT                              0x0
#define SYS_PORT_E_ODC                              0x0

#define SYS_PORT_F_ANSEL                            0xFFFF
#define SYS_PORT_F_TRIS_OUT                         0x8
#define SYS_PORT_F_TRIS_IN                          0x2104
#define SYS_PORT_F_LAT                              0x0
#define SYS_PORT_F_ODC                              0x0

#define SYS_PORT_G_ANSEL                            0xFFFF
#define SYS_PORT_G_TRIS_OUT                         0xE1C0
#define SYS_PORT_G_TRIS_IN                          0x1000
#define SYS_PORT_G_LAT                              0x0
#define SYS_PORT_G_ODC                              0x0

#define SYS_PORT_H_ANSEL                            0xFFFF
#define SYS_PORT_H_TRIS_OUT                         0x4F
#define SYS_PORT_H_TRIS_IN                          0xDE00
#define SYS_PORT_H_LAT                              0x0
#define SYS_PORT_H_ODC                              0x0

#define SYS_PORT_J_ANSEL                            0xFFFF
#define SYS_PORT_J_TRIS_OUT                         0xA0
#define SYS_PORT_J_TRIS_IN                          0x4D
#define SYS_PORT_J_LAT                              0x0
#define SYS_PORT_J_ODC                              0x0

#define SYS_PORT_K_ANSEL                            0xFFFF
#define SYS_PORT_K_TRIS_IN                          0xC0
#define SYS_PORT_K_LAT                              0x0
#define SYS_PORT_K_ODC                              0x0

/*** Timer System Service Configuration ***/
#define SYS_TMR_POWER_STATE             SYS_MODULE_POWER_RUN_FULL
#define SYS_TMR_DRIVER_INDEX            DRV_TMR_INDEX_0
#define SYS_TMR_MAX_CLIENT_OBJECTS      5
#define SYS_TMR_FREQUENCY               1000
#define SYS_TMR_FREQUENCY_TOLERANCE     10
#define SYS_TMR_UNIT_RESOLUTION         10000
#define SYS_TMR_CLIENT_TOLERANCE        10
#define SYS_TMR_INTERRUPT_NOTIFICATION  false

// *****************************************************************************
/* Random System Service Configuration Options
*/

#define SYS_RANDOM_CRYPTO_SEED_SIZE  32


// *****************************************************************************
// *****************************************************************************
// Section: Driver Configuration
// *****************************************************************************
// *****************************************************************************
/*** Timer Driver Configuration ***/
#define DRV_TMR_INTERRUPT_MODE             true
#define DRV_TMR_INSTANCES_NUMBER           1
#define DRV_TMR_CLIENTS_NUMBER             1

/*** Timer Driver 0 Configuration for system timer ***/

#define DRV_TMR_PERIPHERAL_ID_IDX0          TMR_ID_1
#define DRV_TMR_INTERRUPT_SOURCE_IDX0       INT_SOURCE_TIMER_1
#define DRV_TMR_INTERRUPT_VECTOR_IDX0       INT_VECTOR_T1
#define DRV_TMR_ISR_VECTOR_IDX0             _TIMER_1_VECTOR
#define DRV_TMR_INTERRUPT_PRIORITY_IDX0     INT_PRIORITY_LEVEL1
#define DRV_TMR_INTERRUPT_SUB_PRIORITY_IDX0 INT_SUBPRIORITY_LEVEL0
#define DRV_TMR_CLOCK_SOURCE_IDX0           DRV_TMR_CLKSOURCE_INTERNAL
#define DRV_TMR_PRESCALE_IDX0               TMR_PRESCALE_VALUE_256
#define DRV_TMR_OPERATION_MODE_IDX0         DRV_TMR_OPERATION_MODE_16_BIT
#define DRV_TMR_ASYNC_WRITE_ENABLE_IDX0     false
#define DRV_TMR_POWER_STATE_IDX0            SYS_MODULE_POWER_RUN_FULL

/* RS232 Driver */

#define DRV_USART_PERIPHERAL_ID_IDX2                USART_ID_6
#define DRV_USART_OPER_MODE_IDX2                    DRV_USART_OPERATION_MODE_NORMAL
#define DRV_USART_OPER_MODE_DATA_IDX2
#define DRV_USART_INIT_FLAG_WAKE_ON_START_IDX2      false
#define DRV_USART_INIT_FLAG_AUTO_BAUD_IDX2          false
#define DRV_USART_INIT_FLAG_STOP_IN_IDLE_IDX2       false
#define DRV_USART_INIT_FLAGS_IDX2                   0
#define DRV_USART_BRG_CLOCK_IDX2                    100000000
#define DRV_USART_BAUD_RATE_IDX2                    CONFIG_SB3_BAUDRATE
#define DRV_USART_LINE_CNTRL_IDX2                   DRV_USART_LINE_CONTROL_8NONE1
#define DRV_USART_HANDSHAKE_MODE_IDX2               DRV_USART_HANDSHAKE_NONE
#define DRV_USART_XMIT_INT_SRC_IDX2                 INT_SOURCE_USART_6_TRANSMIT
#define DRV_USART_RCV_INT_SRC_IDX2                  INT_SOURCE_USART_6_RECEIVE
#define DRV_USART_ERR_INT_SRC_IDX2                  INT_SOURCE_USART_6_ERROR
#define DRV_USART_XMIT_INT_VECTOR_IDX2              INT_VECTOR_UART6_TX
#define DRV_USART_XMIT_INT_PRIORITY_IDX2            INT_PRIORITY_LEVEL1
#define DRV_USART_XMIT_INT_SUBPRIORITY_IDX2         INT_SUBPRIORITY_LEVEL0
#define DRV_USART_RCV_INT_VECTOR_IDX2               INT_VECTOR_UART6_RX
#define DRV_USART_RCV_INT_PRIORITY_IDX2             INT_PRIORITY_LEVEL1
#define DRV_USART_RCV_INT_SUBPRIORITY_IDX2          INT_SUBPRIORITY_LEVEL0
#define DRV_USART_ERR_INT_VECTOR_IDX2               INT_VECTOR_UART6_FAULT
#define DRV_USART_ERR_INT_PRIORITY_IDX2             INT_PRIORITY_LEVEL1
#define DRV_USART_ERR_INT_SUBPRIORITY_IDX2          INT_SUBPRIORITY_LEVEL0

#define DRV_USART_XMIT_QUEUE_SIZE_IDX2              2
#define DRV_USART_RCV_QUEUE_SIZE_IDX2               2

#define DRV_USART_POWER_STATE_IDX2                  SYS_MODULE_POWER_RUN_FULL

#endif /* _SYSTEM_CONFIG_H */
