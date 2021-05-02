#ifndef SYS_TMR
#define SYS_TMR

#include <stdint.h>
#include <stdbool.h>
typedef uintptr_t SYS_TMR_HANDLE;

typedef void ( * SYS_TMR_CALLBACK ) ( uintptr_t context, uint32_t currTick );

SYS_TMR_HANDLE SYS_TMR_CallbackSingle ( uint32_t periodMs, uintptr_t context, SYS_TMR_CALLBACK callback );
void SYS_TMR_CallbackStop ( SYS_TMR_HANDLE handle );

#endif /* SYS_TMR */