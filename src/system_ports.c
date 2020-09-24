#include "config.h"

#include "system_config.h"
#include "system/ports/sys_ports.h"
#include "peripheral/devcon/plib_devcon.h"
#include "peripheral/ports/plib_ports.h"
#include "peripheral/int/plib_int.h"

void SYS_PORTS_Initialize(void)
{
  /* PORT A Initialization */

  PLIB_PORTS_OpenDrainEnable(PORTS_ID_0, PORT_CHANNEL_A, SYS_PORT_A_ODC);

  PLIB_PORTS_Write(PORTS_ID_0, PORT_CHANNEL_A, SYS_PORT_A_LAT);
  PLIB_PORTS_DirectionOutputSet(PORTS_ID_0, PORT_CHANNEL_A,
                                 SYS_PORT_A_TRIS_OUT);
  PLIB_PORTS_DirectionInputSet(PORTS_ID_0, PORT_CHANNEL_A,
                                SYS_PORT_A_TRIS_IN);
  PLIB_PORTS_ChannelModeSelect(PORTS_ID_0, PORT_CHANNEL_A, SYS_PORT_A_ANSEL,
                               PORTS_PIN_MODE_DIGITAL);

  /* PORT B Initialization */

  PLIB_PORTS_OpenDrainEnable(PORTS_ID_0, PORT_CHANNEL_B, SYS_PORT_B_ODC);

  PLIB_PORTS_Write(PORTS_ID_0, PORT_CHANNEL_B, SYS_PORT_B_LAT);
  PLIB_PORTS_DirectionOutputSet(PORTS_ID_0, PORT_CHANNEL_B,
                                 SYS_PORT_B_TRIS_OUT);
  PLIB_PORTS_ChannelModeSelect(PORTS_ID_0, PORT_CHANNEL_B, SYS_PORT_B_ANSEL,
                               PORTS_PIN_MODE_DIGITAL);

  /* PORT C Initialization */

  PLIB_PORTS_OpenDrainEnable(PORTS_ID_0, PORT_CHANNEL_C, SYS_PORT_C_ODC);

  PLIB_PORTS_Write(PORTS_ID_0, PORT_CHANNEL_C, SYS_PORT_C_LAT);
  PLIB_PORTS_DirectionOutputSet(PORTS_ID_0, PORT_CHANNEL_C,
                                SYS_PORT_C_TRIS_OUT);
  PLIB_PORTS_DirectionInputSet(PORTS_ID_0, PORT_CHANNEL_C,
                                SYS_PORT_C_TRIS_IN);
  PLIB_PORTS_ChannelModeSelect(PORTS_ID_0, PORT_CHANNEL_C, SYS_PORT_C_ANSEL,
                               PORTS_PIN_MODE_DIGITAL);
  PLIB_PORTS_ChannelModeSelect(PORTS_ID_0, PORT_CHANNEL_C, 0xFFFF, PORTS_PIN_MODE_DIGITAL);
  PLIB_PORTS_ChannelModeSelect(PORTS_ID_0, PORT_CHANNEL_B, 0xFFFF, PORTS_PIN_MODE_DIGITAL);

  /* PORT D Initialization */

  PLIB_PORTS_OpenDrainEnable(PORTS_ID_0, PORT_CHANNEL_D, SYS_PORT_D_ODC);

  PLIB_PORTS_Write(PORTS_ID_0, PORT_CHANNEL_D, SYS_PORT_D_LAT);
  PLIB_PORTS_DirectionOutputSet(PORTS_ID_0, PORT_CHANNEL_D,
                                SYS_PORT_D_TRIS_OUT);
  PLIB_PORTS_DirectionInputSet(PORTS_ID_0, PORT_CHANNEL_D,
                                SYS_PORT_D_TRIS_IN);
  PLIB_PORTS_ChannelModeSelect(PORTS_ID_0, PORT_CHANNEL_D, SYS_PORT_D_ANSEL,
                               PORTS_PIN_MODE_DIGITAL);

  /* PORT E Initialization */

  PLIB_PORTS_OpenDrainEnable(PORTS_ID_0, PORT_CHANNEL_E, SYS_PORT_E_ODC);

  PLIB_PORTS_Write(PORTS_ID_0, PORT_CHANNEL_E, SYS_PORT_E_LAT);
  PLIB_PORTS_DirectionOutputSet(PORTS_ID_0, PORT_CHANNEL_E,
                                 SYS_PORT_E_TRIS_OUT);
  PLIB_PORTS_ChannelModeSelect(PORTS_ID_0, PORT_CHANNEL_E, SYS_PORT_E_ANSEL,
                               PORTS_PIN_MODE_DIGITAL);

  /* PORT F Initialization */

  PLIB_PORTS_OpenDrainEnable(PORTS_ID_0, PORT_CHANNEL_F, SYS_PORT_F_ODC);

  PLIB_PORTS_Write(PORTS_ID_0, PORT_CHANNEL_F, SYS_PORT_F_LAT);
  PLIB_PORTS_DirectionOutputSet(PORTS_ID_0, PORT_CHANNEL_F,
                                SYS_PORT_F_TRIS_OUT);
  PLIB_PORTS_DirectionInputSet(PORTS_ID_0, PORT_CHANNEL_F,
                                SYS_PORT_F_TRIS_IN);
  PLIB_PORTS_ChannelModeSelect(PORTS_ID_0, PORT_CHANNEL_F, SYS_PORT_F_ANSEL,
                               PORTS_PIN_MODE_DIGITAL);

  /* PORT G Initialization */

  PLIB_PORTS_OpenDrainEnable(PORTS_ID_0, PORT_CHANNEL_G, SYS_PORT_G_ODC);

  PLIB_PORTS_Write(PORTS_ID_0, PORT_CHANNEL_G, SYS_PORT_G_LAT);
  PLIB_PORTS_DirectionOutputSet(PORTS_ID_0, PORT_CHANNEL_G,
                                 SYS_PORT_G_TRIS_OUT);
  PLIB_PORTS_DirectionInputSet(PORTS_ID_0, PORT_CHANNEL_G,
                                SYS_PORT_G_TRIS_IN);
  PLIB_PORTS_ChannelModeSelect(PORTS_ID_0, PORT_CHANNEL_G, SYS_PORT_G_ANSEL,
                               PORTS_PIN_MODE_DIGITAL);

  /* PORT H Initialization */

  PLIB_PORTS_OpenDrainEnable(PORTS_ID_0, PORT_CHANNEL_H, SYS_PORT_H_ODC);

  PLIB_PORTS_Write(PORTS_ID_0, PORT_CHANNEL_H, SYS_PORT_H_LAT);
  PLIB_PORTS_DirectionOutputSet(PORTS_ID_0, PORT_CHANNEL_H,
                                 SYS_PORT_H_TRIS_OUT);
  PLIB_PORTS_DirectionInputSet(PORTS_ID_0, PORT_CHANNEL_H,
                                SYS_PORT_H_TRIS_IN);
  PLIB_PORTS_ChannelModeSelect(PORTS_ID_0, PORT_CHANNEL_H, SYS_PORT_H_ANSEL,
                               PORTS_PIN_MODE_DIGITAL);

  /* PORT J Initialization */

  PLIB_PORTS_OpenDrainEnable(PORTS_ID_0, PORT_CHANNEL_J, SYS_PORT_J_ODC);

  PLIB_PORTS_Write(PORTS_ID_0, PORT_CHANNEL_J, SYS_PORT_J_LAT);
  PLIB_PORTS_DirectionOutputSet(PORTS_ID_0, PORT_CHANNEL_J,
                                 SYS_PORT_J_TRIS_OUT);
  PLIB_PORTS_DirectionInputSet(PORTS_ID_0, PORT_CHANNEL_J,
                                SYS_PORT_J_TRIS_IN);
  PLIB_PORTS_ChannelModeSelect(PORTS_ID_0, PORT_CHANNEL_J, SYS_PORT_J_ANSEL,
                               PORTS_PIN_MODE_DIGITAL);

  /* PORT J Initialization */

  PLIB_PORTS_OpenDrainEnable(PORTS_ID_0, PORT_CHANNEL_K, SYS_PORT_K_ODC);

  PLIB_PORTS_Write(PORTS_ID_0, PORT_CHANNEL_K, SYS_PORT_K_LAT);
  PLIB_PORTS_DirectionInputSet(PORTS_ID_0, PORT_CHANNEL_K,
                                SYS_PORT_K_TRIS_IN);
  PLIB_PORTS_ChannelModeSelect(PORTS_ID_0, PORT_CHANNEL_K, SYS_PORT_K_ANSEL,
                               PORTS_PIN_MODE_DIGITAL);

  /* Pins Remapping */
 
  /* UART2 */

  PLIB_PORTS_RemapInput(PORTS_ID_0, INPUT_FUNC_U2RX, INPUT_PIN_RPB7);
  PLIB_PORTS_RemapOutput(PORTS_ID_0, OUTPUT_FUNC_U2TX, OUTPUT_PIN_RPB6);

  /* UART4 */

  PLIB_PORTS_RemapInput(PORTS_ID_0, INPUT_FUNC_U4RX, INPUT_PIN_RPB14);
  PLIB_PORTS_RemapOutput(PORTS_ID_0, OUTPUT_FUNC_U4TX, OUTPUT_PIN_RPF12);

  /* UART6 */

  PLIB_PORTS_RemapInput(PORTS_ID_0, INPUT_FUNC_U6RX, INPUT_PIN_RPF2);
  PLIB_PORTS_RemapOutput(PORTS_ID_0, OUTPUT_FUNC_U6TX, OUTPUT_PIN_RPF3);

  /* SPI1 */

  PLIB_PORTS_RemapInput(PORTS_ID_0, INPUT_FUNC_SDI1, INPUT_PIN_RPB10);
  PLIB_PORTS_RemapOutput(PORTS_ID_0, OUTPUT_FUNC_SDO1, OUTPUT_PIN_RPC14);

  /* Don't map the CS PIN. Control it manually. This is for framed SPI.  */
#if 0
  PLIB_PORTS_RemapOutput(PORTS_ID_0, OUTPUT_FUNC_SS1, OUTPUT_PIN_RPB15);
#endif

  /* Input Caputre */

  PLIB_PORTS_RemapInput(PORTS_ID_0, INPUT_FUNC_IC1, INPUT_PIN_RPF13);
  PLIB_PORTS_RemapInput(PORTS_ID_0, INPUT_FUNC_IC2, INPUT_PIN_RPD4);
  PLIB_PORTS_RemapInput(PORTS_ID_0, INPUT_FUNC_IC3, INPUT_PIN_RPD10);
  PLIB_PORTS_RemapInput(PORTS_ID_0, INPUT_FUNC_IC4, INPUT_PIN_RPD15);
  PLIB_PORTS_RemapInput(PORTS_ID_0, INPUT_FUNC_IC5, INPUT_PIN_RPF8);
  PLIB_PORTS_RemapInput(PORTS_ID_0, INPUT_FUNC_IC6, INPUT_PIN_RPD0);
  PLIB_PORTS_RemapInput(PORTS_ID_0, INPUT_FUNC_IC7, INPUT_PIN_RPA14);
  PLIB_PORTS_RemapInput(PORTS_ID_0, INPUT_FUNC_IC8, INPUT_PIN_RPA15);

}

PORTS_DATA_TYPE SYS_PORTS_Read(PORTS_MODULE_ID index, PORTS_CHANNEL channel)
{
  return PLIB_PORTS_Read(index, channel);
}

void SYS_PORTS_Write(PORTS_MODULE_ID index,
                      PORTS_CHANNEL channel,
                      PORTS_DATA_TYPE value)
{
  PLIB_PORTS_Write(index, channel, value);
}

void SYS_PORTS_Set(PORTS_MODULE_ID index, PORTS_CHANNEL channel,
                    PORTS_DATA_TYPE value,
                    PORTS_DATA_MASK mask)
{
  PLIB_PORTS_Set(index, channel, value, mask);
}

void SYS_PORTS_Clear(PORTS_MODULE_ID index, PORTS_CHANNEL channel,
                      PORTS_DATA_MASK clearMask)
{
  PLIB_PORTS_Clear(index, channel, clearMask);
}

void SYS_PORTS_DirectionSelect(PORTS_MODULE_ID index,
                                SYS_PORTS_PIN_DIRECTION pinDir,
                                PORTS_CHANNEL channel,
                                PORTS_DATA_MASK mask)
{
  if (pinDir == SYS_PORTS_DIRECTION_INPUT)
    {
      PLIB_PORTS_DirectionInputSet(index, channel, mask);
    }
  else
    {
      PLIB_PORTS_DirectionOutputSet(index, channel, mask);
    }
}

PORTS_DATA_MASK SYS_PORTS_DirectionGet(PORTS_MODULE_ID index,
                                        PORTS_CHANNEL channel)
{
  return PLIB_PORTS_DirectionGet(index, channel);
}

void SYS_PORTS_Toggle(PORTS_MODULE_ID index, PORTS_CHANNEL channel,
                       PORTS_DATA_MASK toggleMask)
{
  PLIB_PORTS_Toggle(index, channel, toggleMask);
}

void SYS_PORTS_OpenDrainEnable(PORTS_MODULE_ID index, PORTS_CHANNEL channel,
                                PORTS_DATA_MASK mask)
{
#if defined(PLIB_PORTS_ExistsPortsOpenDrain)
  if (PLIB_PORTS_ExistsPortsOpenDrain(index))
    {
      PLIB_PORTS_OpenDrainEnable(index, channel, mask);
    }
#endif
}

void SYS_PORTS_OpenDrainDisable(PORTS_MODULE_ID index, PORTS_CHANNEL channel,
                                 PORTS_DATA_MASK mask)
{
#if defined(PLIB_PORTS_ExistsPortsOpenDrain)
  if (PLIB_PORTS_ExistsPortsOpenDrain(index))
    {
      PLIB_PORTS_OpenDrainDisable(index, channel, mask);
    }
#endif
}

void SYS_PORTS_ChangeNotificationGlobalEnable(PORTS_MODULE_ID index)
{
#if defined(PLIB_PORTS_ExistsChangeNotice)
  if (PLIB_PORTS_ExistsChangeNotice(index))
    {
      PLIB_PORTS_ChangeNoticeEnable(index);
    }
#endif
}

void SYS_PORTS_ChangeNotificationGlobalDisable(PORTS_MODULE_ID index)
{
#if defined(PLIB_PORTS_ExistsChangeNotice)
  if (PLIB_PORTS_ExistsChangeNotice(index))
    {
      PLIB_PORTS_ChangeNoticeDisable(index);
    }
#endif
}

void SYS_PORTS_GlobalChangeNotificationDisable(PORTS_MODULE_ID index)
{
#if defined(PLIB_PORTS_ExistsChangeNotice)
  if (PLIB_PORTS_ExistsChangeNotice(index))
    {
      PLIB_PORTS_ChangeNoticeDisable(index);
    }
#endif
}

void SYS_PORTS_ChangeNotificationEnable(
  PORTS_MODULE_ID index,
  PORTS_CHANGE_NOTICE_PIN pinNum,
  SYS_PORTS_PULLUP_PULLDOWN_STATUS
  value)
{
#if defined(PLIB_PORTS_ExistsChangeNoticePullUp)
  if (PLIB_PORTS_ExistsChangeNoticePullUp(index))
    {
      switch (value)
        {
        case SYS_PORTS_PULLUP_DISABLE:
          PLIB_PORTS_ChangeNoticePullUpDisable(index, pinNum);
          break;

        case SYS_PORTS_PULLUP_ENABLE:
          PLIB_PORTS_ChangeNoticePullUpEnable(index, pinNum);
          break;
        }
    }
#endif

#if defined(PLIB_PORTS_ExistsPinChangeNotice)
  if (PLIB_PORTS_ExistsPinChangeNotice(index))
    {
      PLIB_PORTS_PinChangeNoticeEnable(index, pinNum);
    }
#endif
}

void SYS_PORTS_ChangeNotificationDisable(PORTS_MODULE_ID index,
                                          PORTS_CHANGE_NOTICE_PIN pinNum)
{
#if defined(PLIB_PORTS_ExistsPinChangeNotice)
  if (PLIB_PORTS_ExistsPinChangeNotice(index))
    {
      PLIB_PORTS_PinChangeNoticeDisable(index, pinNum);
    }
#endif
}

void SYS_PORTS_ChangeNotificationInIdleModeEnable(PORTS_MODULE_ID index)
{
#if defined(PLIB_PORTS_ExistsChangeNoticeInIdle)
  if (PLIB_PORTS_ExistsChangeNoticeInIdle(index))
    {
      PLIB_PORTS_ChangeNoticeInIdleEnable(index);
    }
#endif
}

void SYS_PORTS_ChangeNotificationInIdleModeDisable(PORTS_MODULE_ID index)
{
#if defined(PLIB_PORTS_ExistsChangeNoticeInIdle)
  if (PLIB_PORTS_ExistsChangeNoticeInIdle(index))
    {
      PLIB_PORTS_ChangeNoticeInIdleDisable(index);
    }
#endif
}

void SYS_PORTS_ChangeNotificationPullUpEnable(PORTS_MODULE_ID index,
                                               PORTS_CHANGE_NOTICE_PIN pinNum)
{
#if defined(PLIB_PORTS_ExistsChangeNoticePullUp)
  if (PLIB_PORTS_ExistsChangeNoticePullUp(index))
    {
      PLIB_PORTS_ChangeNoticePullUpEnable(index, pinNum);
    }
#endif
}

void SYS_PORTS_ChangeNotificationPullUpDisable(PORTS_MODULE_ID index,
                                               PORTS_CHANGE_NOTICE_PIN
                                               pinNum)
{
#if defined(PLIB_PORTS_ExistsChangeNoticePullUp)
  if (PLIB_PORTS_ExistsChangeNoticePullUp(index))
    {
      PLIB_PORTS_ChangeNoticePullUpDisable(index, pinNum);
    }
#endif
}

void SYS_PORTS_PinModeSelect(PORTS_MODULE_ID index, PORTS_ANALOG_PIN pin,
                              PORTS_PIN_MODE mode)
{
#if defined(PLIB_PORTS_ExistsPinMode)
  if (PLIB_PORTS_ExistsPinMode(index))
    {
      PLIB_PORTS_PinModeSelect(index, pin, mode);
    }
#endif
}

void SYS_PORTS_PinWrite(PORTS_MODULE_ID index,
                         PORTS_CHANNEL channel,
                         PORTS_BIT_POS bitPos,
                         bool value)
{
  PLIB_PORTS_PinWrite(index, channel, bitPos, value);
}

bool SYS_PORTS_PinRead(PORTS_MODULE_ID index,
                        PORTS_CHANNEL channel,
                        PORTS_BIT_POS bitPos)
{
  return PLIB_PORTS_PinGet(index, channel, bitPos);
}

void SYS_PORTS_PinToggle(PORTS_MODULE_ID index,
                          PORTS_CHANNEL channel,
                          PORTS_BIT_POS bitPos)
{
  PLIB_PORTS_PinToggle(index, channel, bitPos);
}

void SYS_PORTS_PinSet(PORTS_MODULE_ID index,
                       PORTS_CHANNEL channel,
                       PORTS_BIT_POS bitPos)
{
  PLIB_PORTS_PinSet(index, channel, bitPos);
}

void SYS_PORTS_PinClear(PORTS_MODULE_ID index,
                         PORTS_CHANNEL channel,
                         PORTS_BIT_POS bitPos)
{
  PLIB_PORTS_PinClear(index, channel, bitPos);
}

void SYS_PORTS_PinDirectionSelect(PORTS_MODULE_ID index,
                                   SYS_PORTS_PIN_DIRECTION pinDir,
                                   PORTS_CHANNEL channel,
                                   PORTS_BIT_POS bitPos)
{
  if (pinDir == SYS_PORTS_DIRECTION_OUTPUT)
    {
      PLIB_PORTS_PinDirectionOutputSet(index, channel, bitPos);
    }
  else
    {
      PLIB_PORTS_PinDirectionInputSet(index, channel, bitPos);
    }
}

void SYS_PORTS_PinOpenDrainEnable(PORTS_MODULE_ID index,
                                   PORTS_CHANNEL channel,
                                   PORTS_BIT_POS bitPos)
{
#if defined(PLIB_PORTS_ExistsPortsOpenDrain)
  if (PLIB_PORTS_ExistsPortsOpenDrain(index))
    {
      PLIB_PORTS_PinOpenDrainEnable(index, channel, bitPos);
    }
#endif
}

void SYS_PORTS_PinOpenDrainDisable(PORTS_MODULE_ID index,
                                    PORTS_CHANNEL channel,
                                    PORTS_BIT_POS bitPos)
{
#if defined(PLIB_PORTS_ExistsPortsOpenDrain)
  if (PLIB_PORTS_ExistsPortsOpenDrain(index))
    {
      PLIB_PORTS_PinOpenDrainDisable(index, channel, bitPos);
    }
#endif
}

void SYS_PORTS_RemapInput(PORTS_MODULE_ID index,
                           PORTS_REMAP_INPUT_FUNCTION function,
                           PORTS_REMAP_INPUT_PIN remapPin)
{
#if defined(PLIB_PORTS_ExistsRemapInput)
  if (PLIB_PORTS_ExistsRemapInput(index))
    {
      PLIB_DEVCON_SystemUnlock(DEVCON_ID_0);
      PLIB_DEVCON_DeviceRegistersUnlock(DEVCON_ID_0, DEVCON_PPS_REGISTERS);
      PLIB_PORTS_RemapInput(index, function, remapPin);
    }
#endif
}

void SYS_PORTS_RemapOutput(PORTS_MODULE_ID index,
                            PORTS_REMAP_OUTPUT_FUNCTION function,
                            PORTS_REMAP_OUTPUT_PIN remapPin)
{
#if defined(PLIB_PORTS_ExistsRemapOutput)
  if (PLIB_PORTS_ExistsRemapOutput(index))
    {
      PLIB_DEVCON_SystemUnlock(DEVCON_ID_0);
      PLIB_DEVCON_DeviceRegistersUnlock(DEVCON_ID_0, DEVCON_PPS_REGISTERS);
      PLIB_PORTS_RemapOutput(index, function, remapPin);
    }
#endif
}

