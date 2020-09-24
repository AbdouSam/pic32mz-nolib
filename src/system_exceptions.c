#include <xc.h>                 /* Defines special funciton registers, CP0
                                 *regs  */
#include "system_config.h"
#include "system_definitions.h"
#include "system/debug/sys_debug.h"

/* *****************************************************************************
 * *****************************************************************************
 * Section: Global Data Definitions
 * *****************************************************************************
 * *****************************************************************************
 */

/*******************************************************************************
 *
 *  Remarks:
 *   These global static items are used instead of local variables in the
 *   _general_exception_handler function because the stack may not be
 * available
 *   if an exception has occured.
 */

/* Code identifying the cause of the exception (CP0 Cause register). */
static unsigned int _excep_code;

/* Address of instruction that caused the exception. */
static unsigned int _excep_addr;

/* Pointer to the string describing the cause of the exception. */
static char *_cause_str;

/* Array identifying the cause (indexed by _exception_code). */
static char *cause[] =
{
  "Interrupt",
  "Undefined",
  "Undefined",
  "Undefined",
  "Load/fetch address error",
  "Store address error",
  "Instruction bus error",
  "Data bus error",
  "Syscall",
  "Breakpoint",
  "Reserved instruction",
  "Coprocessor unusable",
  "Arithmetic overflow",
  "Trap",
  "Reserved",
  "Reserved",
  "Reserved",
  "Reserved",
  "Reserved"
};

/* </editor-fold> */

/* *****************************************************************************
 * *****************************************************************************
 * Section: Exception Handling
 * *****************************************************************************
 * *****************************************************************************
 */

/*******************************************************************************
 *  Function:
 *   void _general_exception_handler ( void )
 *
 *  Summary:
 *   Overrides the XC32 _weak_ _generic_exception_handler.
 *
 *  Description:
 *   This function overrides the XC32 default _weak_
 * _generic_exception_handler.
 *
 *  Remarks:
 *   Refer to the XC32 User's Guide for additional information.
 */

void _general_exception_handler( void )
{
  /* Mask off Mask of the ExcCode Field from the Cause Register
   *  Refer to the MIPs Software User's manual */
  _excep_code   = (_CP0_GET_CAUSE() & 0x0000007C) >> 2;
  _excep_addr   = _CP0_GET_EPC();
  _cause_str    = cause[_excep_code];

  SYS_DEBUG_PRINT(SYS_ERROR_ERROR,
                  "\nGeneral Exception %s (cause=%d, addr=%x).\n",
                  _cause_str, _excep_code,
                  _excep_addr);

  while (1)
    {
      /* SYS_DEBUG_BreakPoint(); */

      SYS_RESET_SoftwareReset();
    }
}

