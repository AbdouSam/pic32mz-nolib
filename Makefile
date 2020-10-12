CROSS_COMPILE ?= xc32-

MCU = 32MZ2048EFM144

INC_DIR = -I./src

OBJ_DIR = build/release/out
BIN_DIR = build/release
SRC_DIR = src
LD_DIR  = scripts
LD_FILE = generic_pic32mz.ld

CSRCS = main.c \
        uart.c \
        gpio.c \
        sysclk.c \
        delay.c \

SRC_C = $(addprefix $(SRC_DIR)/, $(CSRCS))
OBJS  = $(addprefix $(OBJ_DIR)/, $(CSRCS:.c=.o))

CFLAGS = -mprocessor=$(MCU)
CFLAGS += $(INC) -std=gnu99 -O1 -Wfatal-errors -Winline -Wall -no-legacy-libc -finline

MIN_HEAP_SIZE = 0
MIN_STACK_SIZE = 0x400
#Debugging/Optimization
LDFLAGS = -mprocessor=$(MCU)
LDFLAGS+= -no-legacy-libc 
LDFLAGS+= -Wl,--script="$(LD_DIR)/$(LD_FILE)",--gc-sections,--no-code-in-dinit,--no-dinit-in-serial-mem,-Map=$(BIN_DIR)/firmware.map
LDFLAGS+= -Wl,--defsym=_min_heap_size=$(MIN_HEAP_SIZE),--defsym=_min_stack_size=$(MIN_STACK_SIZE)
LIBS = -lc -lm



all: $(BIN_DIR)/firmware.hex

$(BIN_DIR)/firmware.hex: $(BIN_DIR)/firmware.elf
	@echo "Create $@"
	@$(CROSS_COMPILE)bin2hex $<

$(BIN_DIR)/firmware.elf: $(OBJS)
	@echo "LINK $@"
	@$(CROSS_COMPILE)gcc $(LDFLAGS) -o $@ $^ $(LIBS)

$(OBJS): $(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@echo "Compile $< to get $@"
	@$(CROSS_COMPILE)gcc -c -x c $(CFLAGS) $< -o $@  $(LIBS)

.PHONY: clean

clean:
	@rm -f $(OBJS)
	@rm -f $(BIN_DIR)/firmware.hex $(BIN_DIR)/firmware.elf $(BIN_DIR)/firmware.map
	@echo "Cleanup complete."

test:
	@echo "src files: $(SRC_C)"
	@echo "obj files:  $(OBJS)"