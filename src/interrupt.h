#ifndef INCLUDED_PIC32_INTERRUPT_H
#define INCLUDED_PIC32_INTERRUPT_H

#define INT_PRIO_BITLEN (7U)
#define INT_SUBP_BITLEN (3U)

#define IPC_SUBP_SHIFT_0 (0U)
#define IPC_PRIO_SHIFT_0 (2U)

#define IPC_SUBP_SHIFT_8  (8U)
#define IPC_PRIO_SHIFT_8  (10U)

#define IPC_SUBP_SHIFT_16 (16U)
#define IPC_PRIO_SHIFT_16 (18U)

#define IPC_SUBP_SHIFT_24 (24U)
#define IPC_PRIO_SHIFT_24 (26U)

typedef void (*int_callback_ft)(void);

typedef struct
{
  uint32_t volatile * const  flag;
  uint32_t volatile * const  enable;
  uint32_t volatile * const  prio;
  uint32_t prioshift;
  uint32_t subpshift;
  uint8_t defprio;
  uint8_t ibit;
}intr_regs_t;

void interrupt_init(void);
uint32_t interrupt_tick_get(void);
#endif /* INCLUDED_PIC32_INTERRUPT_H */