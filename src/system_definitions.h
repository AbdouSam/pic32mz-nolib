#ifndef _SYSTEM_DEFINITIONS_H
#define _SYSTEM_DEFINITIONS_H

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <string.h>

#include "config.h"

#include "system/common/sys_common.h"
#include "system/common/sys_module.h"
#include "system/clk/sys_clk.h"
#include "system/devcon/sys_devcon.h"
#include "system/int/sys_int.h"
#include "system/random/sys_random.h"
#include "system/tmr/sys_tmr.h"
#include "system/reset/sys_reset.h"
#include "driver/tmr/drv_tmr.h"
#include "bootloader/src/bootloader.h"
#include "system/ports/sys_ports.h"
#include "driver/ethmac/drv_ethmac.h"
#include "tcpip/tcpip.h"

#include "btlder.h"

typedef struct
{
  SYS_MODULE_OBJ  sysDevcon;
  SYS_MODULE_OBJ  sysTmr;
  SYS_MODULE_OBJ  drvTmr0;
  SYS_MODULE_OBJ  tcpip;
} SYSTEM_OBJECTS;

extern SYSTEM_OBJECTS sysObj;

#endif /* _SYSTEM_DEFINITIONS_H */
