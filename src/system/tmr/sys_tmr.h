#ifndef SYS_TMR
#define SYS_TMR

#include <stdint.h>
#include <stdbool.h>

typedef void (*cb_ft)(void);

void SYS_TMR_CallbackSingle ( uint32_t periodMs, uintptr_t context, cb_ft callback );

#endif /* SYS_TMR */