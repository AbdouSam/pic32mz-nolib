# Minimal code for PIC32

A minimal source code to start working with pic32mz, without the use of any library (no Harmony).

## configuration

Before start, make sure to adjust the pic32_config.h header file.

**GPIO**

- pic32mz pin count, will automatically ignore non-exsistant gpio ports
- set the initial state of gpios, as input or output. this inital config is
  applied if `gpio_init` is called, otherwise all pins can be set input or output
  in run time using `gpio_input_set` and `gpio_output_set`
- pins used for functions, such as _UART_ are adjusted automatically in `uart_init`

**TIMERS**

- pic32mz have 9 timers, timer1 being different from the 8 other timers, it has its own init func
- timer 1 is used to generate the tick, therefore cannot be disabled from config header. 
- enabling a timer will automatically enable its interrupt


**UARTs**

- picmz have 6 UARTS, and the mapping for uart pins rx and tx should be chosed from the datasheet
  because only a set of pins can be used for rx or tx for any specifig UART
- enabling a UART will make it available for use
- `PIC32_UART_x_RX_PIN` and `PIC32_UART_x_RX_PIN` should be set for any enabled UART


**INTERRUPT PRIORITY**

priorities for peripherals are set in the config file, thus, they are set in compile time.

the reason is, for better performance pic32mz uses a set of Shadow Registers to save the context of the CPU
during interrupts. Each shadow register is related to an interrupt priority. 
for example interrupts with priority 5, will use shadow register 5 to save their context.
setting the Shadow register for a given interrupt vector is set in compile time in the definition of the isr by the `iplxAUTO` keyword.


## application

by default tha main function calls two functions, `app_init` one time before the infinite loop and `app_task()` in an infinite loop

`app.c` provide a simple example of use.


## building

the build is done by calling  `make`, preferably call `make clean` before.

### Makefile

there are several variables to addapt to the MCU used or the way how to program

`MCU` : set the name of the mcu used
`LD_SCRIPT` : takes either **custom** to use a customed linker script, in case there is a bootloader in the board
or set it to nothing to use the default linker script from the compiler.

`CSRCS` : if any source file is added to the project, the source file should be added in the list

## programming

if the default linker script from the compiler is used, the programing is done with a pickit

if the custom liker script is used, a booloader should be burned in the MCU to be able to upload code using UART or UDP

there are examples of bootloader from the harmony library.

if a bootloader is in place, the script in `tools/programmer.py` can be used to upload the code through UART or UDP. 

