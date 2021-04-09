CROSS_COMPILE ?= xc32-

MCU = 32MZ2048EFM144

BIN_DIR = build/release
OBJ_DIR = build/release/out
SRC_DIR = src

FREERTOS_DIR = src/FreeRTOS/Source
FREERTOS_MEM_DIR = $(FREERTOS_DIR)/portable/MemMang
FREERTOS_PORT_DIR = $(FREERTOS_DIR)/portable/MPLAB/PIC32MZ

INC_DIR = -I./src
INC_DIR += -I./$(FREERTOS_DIR)/include
INC_DIR += -I./$(FREERTOS_DIR)/portable/MPLAB/PIC32MZ


LD_DIR  = scripts
LD_FILE = generic_pic32mz.ld
CFG_FILE = pic32_config.h

CSRCS = main.c \
        uart.c \
        gpio.c \
        sysclk.c \
        delay.c \
        timer.c \
        interrupt.c \
        i2c.c \
        rtc.c \

SRC_C = $(addprefix $(SRC_DIR)/, $(CSRCS))

CSRCS_FREERTOS = list.c \
					       queue.c \
					       tasks.c \
					       timers.c \
					       


# FreeRTOS objects

OBJSRTOS  = $(addprefix $(OBJ_DIR)/, $(CSRCS_FREERTOS:.c=.o))
	
OBJSRTOSASM  = $(OBJ_DIR)/port_asm.o

OBJSRTOSMEM = $(OBJ_DIR)/heap_4.o

OBJSRTOSPORT = $(OBJ_DIR)/port.o

# Project objects

OBJS  = $(addprefix $(OBJ_DIR)/, $(CSRCS:.c=.o))

CFLAGS = -mprocessor=$(MCU)
CFLAGS += $(INC_DIR) -std=gnu99 -O1 -Wfatal-errors -Winline -Wall -no-legacy-libc -finline

ASFLAGS = -mprocessor=$(MCU) $(INC_DIR) -no-legacy-libc

MIN_HEAP_SIZE = 0
MIN_STACK_SIZE = 0x400
#Debugging/Optimization
LDFLAGS = -mprocessor=$(MCU)
LDFLAGS+= -no-legacy-libc 
LDFLAGS+= -Wl,--script="$(LD_DIR)/$(LD_FILE)",--gc-sections,--no-code-in-dinit,--no-dinit-in-serial-mem,-Map=$(BIN_DIR)/firmware.map
LDFLAGS+= -Wl,--defsym=_min_heap_size=$(MIN_HEAP_SIZE),--defsym=_min_stack_size=$(MIN_STACK_SIZE)
LIBS = -lc -lm

.PHONY: clean all

all: $(BIN_DIR)/firmware.hex

$(BIN_DIR)/firmware.hex: $(BIN_DIR)/firmware.elf
	@echo "Create $@"
	@$(CROSS_COMPILE)bin2hex $<

$(BIN_DIR)/firmware.elf: $(OBJS) $(OBJSRTOS) $(OBJSRTOSASM) $(OBJSRTOSMEM) $(OBJSRTOSPORT) | $(shell mkdir -p $(OBJ_DIR))
	@echo "LINK $@"
	@$(CROSS_COMPILE)gcc $(LDFLAGS) -o $@ $^ $(LIBS)

$(OBJS): $(OBJ_DIR)/%.o: $(SRC_DIR)/%.c $(SRC_DIR)/$(CFG_FILE) 
	@echo "Compile $< to get $@"
	@$(CROSS_COMPILE)gcc -c -x c $(CFLAGS) $< -o $@  $(LIBS)

$(OBJSRTOS): $(OBJ_DIR)/%.o :  $(FREERTOS_DIR)/%.c
	@echo "Compile $< to get $@"
	@$(CROSS_COMPILE)gcc -c -x c $(CFLAGS) $< -o $@  $(LIBS)

$(OBJSRTOSMEM): $(FREERTOS_MEM_DIR)/heap_4.c
	@echo "Compile $< to get $@"
	@$(CROSS_COMPILE)gcc -c -x c $(CFLAGS) $< -o $@  $(LIBS)

$(OBJSRTOSPORT): $(FREERTOS_PORT_DIR)/port.c
	@echo "Compile $< to get $@"
	@$(CROSS_COMPILE)gcc -c -x c $(CFLAGS) $< -o $@  $(LIBS)

$(OBJSRTOSASM): $(FREERTOS_PORT_DIR)/port_asm.S
	@echo "Compile $< to get $@"
	@$(CROSS_COMPILE)gcc -c  $< -o $@ $(ASFLAGS)



clean:
	@rm -f $(OBJS)
	@rm -f $(OBJSRTOS)
	@rm -f $(OBJSRTOSASM)
	@rm -f $(OBJSRTOSMEM)
	@rm -f $(OBJSRTOSPORT)
	@rm -f $(BIN_DIR)/firmware.hex $(BIN_DIR)/firmware.elf $(BIN_DIR)/firmware.map
	@echo "Cleanup complete."

# watchout, this routine uncrustify all files in src directory
uncrustify:
	@uncrustify -c uncrustify.cfg --no-backup src/* --suffix

printfiles:
	@echo "src files: $(SRC_C)"
	@echo "obj files:  $(OBJS)"