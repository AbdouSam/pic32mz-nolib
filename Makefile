CROSS_COMPILE ?= xc32-

MCU = 32MZ2048EFM144
LD_SCRIPT = custom

INC_DIR = -I./src

OBJ_DIR = build/release/out
BIN_DIR = build/release
SRC_DIR = src
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
        app.c \
        debug.c \
        rtc_controller.c \
        adc_controller.c \
        dio.c \
        pivot21.c \
        debug.c \

OBJ_SYS_TMR = $(OBJ_DIR)/sys_tmr.o
SRC_SYS_TMR = $(SRC_DIR)/system/tmr/sys_tmr.c

SRC_C = $(addprefix $(SRC_DIR)/, $(CSRCS))
OBJS  = $(addprefix $(OBJ_DIR)/, $(CSRCS:.c=.o))

CFLAGS = -mprocessor=$(MCU)
CFLAGS += $(INC_DIR) -std=gnu99 -O1 -Wfatal-errors -Winline -Wall -no-legacy-libc -finline

MIN_HEAP_SIZE = 0
MIN_STACK_SIZE = 0x400

LDFLAGS = -mprocessor=$(MCU)
LDFLAGS+= -no-legacy-libc 

ifeq ($(LD_SCRIPT), custom)
LDFLAGS+= -Wl,--script="$(LD_DIR)/$(LD_FILE)"
endif

LDFLAGS+= -Wl,--gc-sections,--no-code-in-dinit,--no-dinit-in-serial-mem,-Map=$(BIN_DIR)/firmware.map
LDFLAGS+= -Wl,--defsym=_min_heap_size=$(MIN_HEAP_SIZE),--defsym=_min_stack_size=$(MIN_STACK_SIZE)
LIBS = -lc -lm



all: $(BIN_DIR)/firmware.hex

$(BIN_DIR)/firmware.hex: $(BIN_DIR)/firmware.elf
	@echo "Create $@"
	@$(CROSS_COMPILE)bin2hex $<

$(BIN_DIR)/firmware.elf: $(OBJS)
	@echo "LINK $@"
	@$(CROSS_COMPILE)gcc $(LDFLAGS) -o $@ $^ $(LIBS)

$(OBJS): $(OBJ_DIR)/%.o: $(SRC_DIR)/%.c $(SRC_DIR)/$(CFG_FILE) $(OBJ_SYS_TMR) $(shell mkdir -p $(OBJ_DIR))
	@echo "Compile $< to get $@"
	@$(CROSS_COMPILE)gcc -c -x c $(CFLAGS) $< -o $@  $(LIBS)

$(OBJ_SYS_TMR): $(SRC_SYS_TMR)
	@echo "Compile $< to get $@"
	@$(CROSS_COMPILE)gcc -c -x c $(CFLAGS) $< -o $@  $(LIBS)

.PHONY: clean all program

clean:
	@rm -f $(OBJS)
	@rm -f $(OBJ_SYS_TMR)
	@rm -f $(BIN_DIR)/firmware.hex $(BIN_DIR)/firmware.elf $(BIN_DIR)/firmware.map
	@echo "Cleanup complete."

# watchout, this routine uncrustify all files in src directory
uncrustify:
	@uncrustify -c uncrustify_deo.cfg --no-backup src/pivot21.c

printfiles:
	@echo "src files: $(SRC_C)"
	@echo "obj files:  $(OBJS)"

program: all
	@sleep 1
	@python3 tools/programmer.py -i udp -a 192.168.1.60 -e
	@sleep 1
	@python3 tools/programmer.py -i udp -a 192.168.1.60 -u build/release/firmware.hex
	@sleep 1
	@python3 tools/programmer.py -i udp -a 192.168.1.60 -r
