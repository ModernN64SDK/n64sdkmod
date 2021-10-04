# Makefile to build hvqsample

#  Kind of HVQ2 decoder being used
#    0 : CPU version
#    1 : CPU+RSP version
USE_RSP = 1

#  Format of display-use frame buffer (Selected HVQ2 decoder also reflects on this)
#    1 : 16-bit
#    2 : 32-bit
CFB_FORMAT = 1

#  Maximum size of image [pixel]  (Reflects on the memory reserved for hvqwork.c)
MAXWIDTH  = 320
MAXHEIGHT = 240

TARGET_STRING := sample
TARGET := $(TARGET_STRING)

# Preprocessor definitions
DEFINES := _FINALROM=1 NDEBUG=1 F3DEX_GBI_2=1 MAXWIDTH=$(MAXWIDTH) MAXHEIGHT=$(MAXHEIGHT) CFB_FORMAT=$(CFB_FORMAT) USE_RSP=$(USE_RSP)

SRC_DIRS :=

# Whether to hide commands or not
VERBOSE ?= 0
ifeq ($(VERBOSE),0)
  V := @
endif

# Whether to colorize build messages
COLOR ?= 1

#==============================================================================#
# Target Executable and Sources                                                #
#==============================================================================#
# BUILD_DIR is the location where all build artifacts are placed
BUILD_DIR := build
ROM            := $(BUILD_DIR)/$(TARGET_STRING).z64
ELF            := $(BUILD_DIR)/$(TARGET_STRING).elf
LD_SCRIPT      := $(TARGET_STRING).ld
BOOT		:= /usr/lib/n64/PR/bootcode/boot.6102
BOOT_OBJ	:= $(BUILD_DIR)/boot.6102.o

# Directories containing source files
SRC_DIRS += src src/buffers src/main asm

C_FILES           := $(foreach dir,$(SRC_DIRS),$(wildcard $(dir)/*.c))
S_FILES           := $(foreach dir,$(SRC_DIRS),$(wildcard $(dir)/*.s))

# Object files
O_FILES := $(foreach file,$(C_FILES),$(BUILD_DIR)/$(file:.c=.o)) \
           $(foreach file,$(S_FILES),$(BUILD_DIR)/$(file:.s=.o)) \
		   $(BOOT_OBJ)

HVQ_FILES           := $(wildcard assets/*.hvq)
HVQ_OBJ_FILES := $(foreach file,$(HVQ_FILES),$(BUILD_DIR)/$(file:.hvq=.hvq.o))

# Automatic dependency files
DEP_FILES := $(O_FILES:.o=.d) $(ASM_O_FILES:.o=.d)  $(BUILD_DIR)/$(LD_SCRIPT).d

#==============================================================================#
# Compiler Options                                                             #
#==============================================================================#

AS        := mips-n64-as
CC        := mips-n64-gcc
CPP       := cpp
LD        := mips-n64-ld
AR        := mips-n64-ar
OBJDUMP   := mips-n64-objdump
OBJCOPY   := mips-n64-objcopy

INCLUDE_DIRS += /usr/include/n64 /usr/include/n64/hvq include $(BUILD_DIR) $(BUILD_DIR)/include src .

C_DEFINES := $(foreach d,$(DEFINES),-D$(d))
DEF_INC_CFLAGS := $(foreach i,$(INCLUDE_DIRS),-I$(i)) $(C_DEFINES)

CFLAGS = -G 0 -O2 -g -mabi=32 -ffreestanding -mfix4300 $(DEF_INC_CFLAGS)
ASFLAGS     := -march=vr4300 -mabi=32 $(foreach i,$(INCLUDE_DIRS),-I$(i)) $(foreach d,$(DEFINES),--defsym $(d))

# C preprocessor flags
CPPFLAGS := -P -Wno-trigraphs $(DEF_INC_CFLAGS)

# tools
PRINT = printf

ifeq ($(COLOR),1)
NO_COL  := \033[0m
RED     := \033[0;31m
GREEN   := \033[0;32m
BLUE    := \033[0;34m
YELLOW  := \033[0;33m
BLINK   := \033[33;5m
endif

# Common build print status function
define print
  @$(PRINT) "$(GREEN)$(1) $(YELLOW)$(2)$(GREEN) -> $(BLUE)$(3)$(NO_COL)\n"
endef

#==============================================================================#
# Main Targets                                                                 #
#==============================================================================#

# Default target
default: $(ROM)

clean:
	$(RM) -r $(BUILD_DIR)

ALL_DIRS := $(BUILD_DIR) $(BUILD_DIR)/assets $(addprefix $(BUILD_DIR)/,$(SRC_DIRS))

# Make sure build directory exists before compiling anything
DUMMY != mkdir -p $(ALL_DIRS)

#==============================================================================#
# Compilation Recipes                                                          #
#==============================================================================#

# Compile C code
$(BUILD_DIR)/%.o: %.c
	$(call print,Compiling:,$<,$@)
	$(V)$(CC) -c $(CFLAGS) -MMD -MF $(BUILD_DIR)/$*.d  -o $@ $<
$(BUILD_DIR)/%.o: $(BUILD_DIR)/%.c
	$(call print,Compiling:,$<,$@)
	$(V)$(CC) -c $(CFLAGS) -MMD -MF $(BUILD_DIR)/$*.d  -o $@ $<

# Assemble assembly code
$(BUILD_DIR)/%.o: %.s
	$(call print,Assembling:,$<,$@)
	$(V)$(CC) -c $(CFLAGS) $(foreach i,$(INCLUDE_DIRS),-Wa,-I$(i)) -x assembler-with-cpp -MMD -MF $(BUILD_DIR)/$*.d  -o $@ $<

# Run linker script through the C preprocessor
$(BUILD_DIR)/$(LD_SCRIPT): $(LD_SCRIPT)
	$(call print,Preprocessing linker script:,$<,$@)
	$(V)$(CPP) $(CPPFLAGS) -DBUILD_DIR=$(BUILD_DIR) -MMD -MP -MT $@ -MF $@.d -o $@ $<

$(BOOT_OBJ): $(BOOT)
	$(V)$(OBJCOPY) -I binary -B mips -O elf32-bigmips $< $@

# convert binary hvq to object file
$(BUILD_DIR)/assets/%.hvq.o: assets/%.hvq
	$(call print,Converting HVQ to ELF:,$<,$@)
	$(V)$(LD) -r -b binary $< -o $@

# Link final ELF file
$(ELF): $(O_FILES) $(HVQ_OBJ_FILES) $(BUILD_DIR)/$(LD_SCRIPT)
	@$(PRINT) "$(GREEN)Linking ELF file:  $(BLUE)$@ $(NO_COL)\n"
	$(V)$(LD) -L $(BUILD_DIR) -T $(BUILD_DIR)/$(LD_SCRIPT) -Map $(BUILD_DIR)/$(TARGET).map --no-check-sections -o $@ $(O_FILES) -L/usr/lib/n64 -lultra_rom -L$(N64_LIBGCCDIR) -lgcc -L/usr/lib/n64/hvq -lhvq2

# Build ROM
$(ROM): $(ELF)
	$(call print,Building ROM:,$<,$@)
	$(V)$(OBJCOPY) --pad-to=0x100000 --gap-fill=0xFF $< $@ -O binary
	$(V)makemask $@

.PHONY: clean default
# with no prerequisites, .SECONDARY causes no intermediate target to be removed
.SECONDARY:

# Remove built-in rules, to improve performance
MAKEFLAGS += --no-builtin-rules

-include $(DEP_FILES)

print-% : ; $(info $* is a $(flavor $*) variable set to [$($*)]) @true
