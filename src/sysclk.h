#ifndef INCLUDED_PIC32_SYSCLK_H
#define INCLUDED_PIC32_SYSCLK_H

#include <stdint.h>

#define PERPHERAL1_CLK_DIV  (1)
#define PERPHERAL2_CLK_DIV  (1)
#define PERPHERAL3_CLK_DIV  (7)
#define PERPHERAL4_CLK_DIV  (1)
#define PERPHERAL5_CLK_DIV  (1)
#define PERPHERAL7_CLK_DIV  (0)
#define PERPHERAL8_CLK_DIV  (1)

#define SYS_FREQ            200000000 /* 200MHz */

uint32_t sysclk_sysfreq_get(void);

/**
 * @brief get the frequency of counting for timers
 * Note that Timers OSC source is PBCLK3, has max DIV = 7
 * the SYS_FREQ is devided by 8 at max. For 200MHz of sys_freq the range
 * of timers is from 40ns at tmr_prescal = 1 with pr = 1 
 * to 670ms at tmr_prescal = 256 with pr = 65535 (max of 16 bits)
 */

uint32_t sysclk_timerfreq_get(void);

uint32_t sysclk_uartfreq_get(void);

void sysclk_init(void);

#endif /* INCLUDED_PIC32_SYSCLK_H */