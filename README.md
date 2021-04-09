# Minimal code for PIC32MZ

A minimal source code to start working with pic32mz, without the use of any library (no Harmony).

## configuration

The source code is tested with a pic32mz2048EFM144 microcontroller

the config file _pic32_config.h_ contains the initial config for :

- all the GPIOs for different pin count for the pic32mz and this is set by the define :
  `#define PIC32_PIN_COUNT 144`
  The config set in the config file is taken into account, only if the gpio_init() is called
  other wise the gpios can be set to input or output in runtime.

- Timers (all 9 timers for pic32mz efm serie), with thiere interrupt periority

- UART (all 6 UARTs) and the PPS mapping for the uart. (see below for more details.)

## Freertos

Adapt a project to use Freertos for PIC32

- download Freertos

- src/interrupt.c : Remove the timer one callback, it is used by FreeRTOS
- src/main.c : use an example of creating a task and semaphore
- use a costum "FreeRTOSConfig.h" to overload default defines,you find it in examples of repo
- Makefile : Add the freertos folder for different folders