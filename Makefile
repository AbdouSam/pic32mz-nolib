CROSS_COMPILE ?= xc32-

MCU = 32MZ2048EFM144
LD_SCRIPT = custom

INC_DIR = -I./src
INC_DIR += -I./iecsrc
INC_DIR += -I $(MATIEC_C_INCLUDE_PATH)	

OBJ_DIR = build/release/out
BIN_DIR = build/release
SRC_DIR = src
SRCIEC_DIR = srciec
LD_DIR  = scripts
LD_FILE = generic_pic32mz.ld
CFG_FILE = pic32_config.h
STFILE = st_file.st
STFILE_DIR = stfile
CSRCS = $(wildcard $(SRC_DIR)/*.c)
#CSRCS_IEC = $(wildcard $(SRCIEC_DIR)/*.c)
CSRCSIEC = Config0.c Res0.c

CSRCS_IEC = $(addprefix $(SRCIEC_DIR)/, $(CSRCSIEC))
SRC_C = $(addprefix $(SRC_DIR)/, $(CSRCS))

OBJS = $(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.o,$(CSRCS))
OBJSIEC = $(patsubst $(SRCIEC_DIR)/%.c,$(OBJ_DIR)/%.o,$(CSRCS_IEC))


CFLAGS = -mprocessor=$(MCU)
CFLAGS += $(INC_DIR) -std=gnu99 -O1 -Wfatal-errors -Winline -Wall -no-legacy-libc -finline

MIN_HEAP_SIZE = 0
MIN_STACK_SIZE = 0x400
#Debugging/Optimization
LDFLAGS = -mprocessor=$(MCU)
LDFLAGS+= -no-legacy-libc 

ifeq ($(LD_SCRIPT), custom)
LDFLAGS+= -Wl,--script="$(LD_DIR)/$(LD_FILE)"
endif

LDFLAGS+= -Wl,--gc-sections,--no-code-in-dinit,--no-dinit-in-serial-mem,-Map=$(BIN_DIR)/firmware.map
LDFLAGS+= -Wl,--defsym=_min_heap_size=$(MIN_HEAP_SIZE),--defsym=_min_stack_size=$(MIN_STACK_SIZE)
LIBS = -lc -lm


all: iec_files $(BIN_DIR)/firmware.hex 

$(BIN_DIR)/firmware.hex:  $(BIN_DIR)/firmware.elf 
	@echo "Create $@"
	@$(CROSS_COMPILE)bin2hex $<

$(BIN_DIR)/firmware.elf: $(OBJS) $(OBJSIEC) 
	@echo "LINK $@"
	@$(CROSS_COMPILE)gcc $(LDFLAGS) -o $@ $^ $(LIBS)

$(OBJS): $(OBJ_DIR)/%.o: $(SRC_DIR)/%.c $(SRC_DIR)/$(CFG_FILE) $(shell mkdir -p $(OBJ_DIR))
	@echo "Compile $< to get $@"
	@$(CROSS_COMPILE)gcc -c -x c $(CFLAGS) $< -o $@  $(LIBS)

$(OBJSIEC): $(OBJ_DIR)/%.o: $(SRCIEC_DIR)/%.c
	@echo "Compile $< to get $@"
	@$(CROSS_COMPILE)gcc -c -x c $(CFLAGS) $< -o $@  $(LIBS);\

iec_files: $(STFILE_DIR)/$(STFILE)
	cd $(SRCIEC_DIR);	iec2c ../$(STFILE_DIR)/$(STFILE) -I $(MATIEC_INCLUDE_PATH)	

.PHONY: clean all program st_file

clean:
	@rm -f $(OBJS)
	@rm -f $(OBJSIEC)
	@rm -f $(BIN_DIR)/firmware.hex $(BIN_DIR)/firmware.elf $(BIN_DIR)/firmware.map
	@rm -f $(SRCIEC_DIR)/*
	@echo "Cleanup complete."

# watchout, this routine uncrustify all files in src directory
uncrustify:
	@uncrustify -c uncrustify.cfg --no-backup src/* --suffix

printfiles:
	@echo "src files: $(SRC_C)"
	@echo "obj files:  $(OBJS)"
	@echo "iec obj files:  $(OBJSIEC)"
	@echo "iec src files:  $(CSRCS_IEC)"

program: all
	@sleep 1
	@python tools/programmer.py -i udp -a 192.168.1.60 -e
	@sleep 1
	@python tools/programmer.py -i udp -a 192.168.1.60 -u build/release/firmware.hex
	@sleep 1
	@python tools/programmer.py -i udp -a 192.168.1.60 -r
