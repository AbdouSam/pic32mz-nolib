#include "system_config.h"
#include "system_definitions.h"
#include "peripheral/osc/plib_osc.h"
#include "system/devcon/sys_devcon.h"

void SYS_CLK_Initialize( const SYS_CLK_INIT const *clkInit )
{
  SYS_DEVCON_SystemUnlock( );

  PLIB_OSC_FRCDivisorSelect( OSC_ID_0, OSC_FRC_DIV_1);

  /* Enable Peripheral Bus 1 */
  PLIB_OSC_PBClockDivisorSet(OSC_ID_0, 0, 2 );
  PLIB_OSC_PBOutputClockEnable(OSC_ID_0, 0 );

  /* Enable Peripheral Bus 2 */
  PLIB_OSC_PBClockDivisorSet(OSC_ID_0, 1, 2 );
  PLIB_OSC_PBOutputClockEnable(OSC_ID_0, 1 );

  /* Enable Peripheral Bus 3 */
  PLIB_OSC_PBClockDivisorSet(OSC_ID_0, 2, 5 );
  PLIB_OSC_PBOutputClockEnable(OSC_ID_0, 2 );

  /* Enable Peripheral Bus 4 */
  PLIB_OSC_PBClockDivisorSet(OSC_ID_0, 3, 2 );
  PLIB_OSC_PBOutputClockEnable(OSC_ID_0, 3 );

  /* Enable Peripheral Bus 5 */
  PLIB_OSC_PBClockDivisorSet(OSC_ID_0, 4, 2 );
  PLIB_OSC_PBOutputClockEnable(OSC_ID_0, 4 );

  /* Enable Peripheral Bus 7 */
  PLIB_OSC_PBClockDivisorSet(OSC_ID_0, 6, 1 );
  PLIB_OSC_PBOutputClockEnable(OSC_ID_0, 6 );

  /* Enable Peripheral Bus 8 */
  PLIB_OSC_PBClockDivisorSet(OSC_ID_0, 7, 2 );
  PLIB_OSC_PBOutputClockEnable(OSC_ID_0, 7 );

  /* Disable REFCLKO1*/
  PLIB_OSC_ReferenceOscDisable( OSC_ID_0, OSC_REFERENCE_1 );

  /* Disable REFCLK1_OE*/
  PLIB_OSC_ReferenceOutputDisable( OSC_ID_0, OSC_REFERENCE_1 );

  /* Disable REFCLKO2*/
  PLIB_OSC_ReferenceOscDisable( OSC_ID_0, OSC_REFERENCE_2 );

  /* Disable REFCLK2_OE*/
  PLIB_OSC_ReferenceOutputDisable( OSC_ID_0, OSC_REFERENCE_2 );

  /* Disable REFCLKO3*/
  PLIB_OSC_ReferenceOscDisable( OSC_ID_0, OSC_REFERENCE_3 );

  /* Disable REFCLK3_OE*/
  PLIB_OSC_ReferenceOutputDisable( OSC_ID_0, OSC_REFERENCE_3 );

  /* Disable REFCLKO4*/
  PLIB_OSC_ReferenceOscDisable( OSC_ID_0, OSC_REFERENCE_4 );

  /* Disable REFCLK4_OE*/
  PLIB_OSC_ReferenceOutputDisable( OSC_ID_0, OSC_REFERENCE_4 );

  SYS_DEVCON_SystemLock( );
}

inline uint32_t SYS_CLK_SystemFrequencyGet( void )
{
  return SYS_CLK_FREQ;
}

inline uint32_t SYS_CLK_PeripheralFrequencyGet(
  CLK_BUSES_PERIPHERAL peripheralBus )
{
  uint32_t freq = 0;

  switch (peripheralBus)
    {
    case CLK_BUS_PERIPHERAL_1:
      freq = SYS_CLK_BUS_PERIPHERAL_1;
      break;

    case CLK_BUS_PERIPHERAL_2:
      freq = SYS_CLK_BUS_PERIPHERAL_2;
      break;

    case CLK_BUS_PERIPHERAL_3:
      freq = SYS_CLK_BUS_PERIPHERAL_3;
      break;

    case CLK_BUS_PERIPHERAL_4:
      freq = SYS_CLK_BUS_PERIPHERAL_4;
      break;

    case CLK_BUS_PERIPHERAL_5:
      freq = SYS_CLK_BUS_PERIPHERAL_5;
      break;

    case CLK_BUS_PERIPHERAL_6:
      break;

    case CLK_BUS_PERIPHERAL_7:
      freq = SYS_CLK_BUS_PERIPHERAL_7;
      break;

    case CLK_BUS_PERIPHERAL_8:
      freq = SYS_CLK_BUS_PERIPHERAL_8;
      break;

    default:
      break;
    }

  return freq;
}

inline uint32_t SYS_CLK_ReferenceClockFrequencyGet(
  CLK_BUSES_REFERENCE referenceBus )
{
  uint32_t freq = 0;

  switch (referenceBus)
    {
    case CLK_BUS_REFERENCE_1:
      break;

    case CLK_BUS_REFERENCE_2:
      break;

    case CLK_BUS_REFERENCE_3:
      break;

    case CLK_BUS_REFERENCE_4:
      break;

    case CLK_BUS_REFERENCE_5:
      break;

    default:
      break;
    }

  return freq;
}

void SYS_CLK_SecondaryOscillatorEnable( void )
{
  /* Check for secondary oscillator status */
  if (!PLIB_OSC_SecondaryIsEnabled(OSC_ID_0))
    {
      /* Unlock and enable secondary oscillator */
      SYS_DEVCON_SystemUnlock();

      PLIB_OSC_SecondaryEnable(OSC_ID_0);

      SYS_DEVCON_SystemLock();
    }
}

void SYS_CLK_SecondaryOscillatorDisable( void )
{
  /* Check for secondary oscillator status */
  if (PLIB_OSC_SecondaryIsEnabled(OSC_ID_0))
    {
      /* Unlock and disable secondary oscillator*/
      SYS_DEVCON_SystemUnlock();

      PLIB_OSC_SecondaryDisable(OSC_ID_0);

      SYS_DEVCON_SystemLock();
    }
}

bool SYS_CLK_SecondaryOscillatorIsEnabled( void )
{
  return PLIB_OSC_SecondaryIsEnabled(OSC_ID_0);
}

