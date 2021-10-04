# Makefile to build polyline (Segments)

TARGET_STRING := polyline
TARGET := $(TARGET_STRING)

# Preprocessor definitions
DEFINES := _FINALROM=1 NDEBUG=1 F3DEX_GBI_2=1

SRC_DIRS :=

TOOLS_DIR := tools

# Whether to hide commands or not
VERBOSE ?= 0
ifeq ($(VERBOSE),0)
  V := @
endif

# Whether to colorize build messages
COLOR ?= 1

ifeq ($(filter clean distclean print-%,$(MAKECMDGOALS)),)
  # Make tools if out of date
  $(info Building tools...)
  DUMMY != $(MAKE) -s -C $(TOOLS_DIR) >&2 || echo FAIL
    ifeq ($(DUMMY),FAIL)
      $(error Failed to build tools)
    endif
  $(info Building ROM...)
endif

#==============================================================================#
# Target Executable and Sources                                                #
#==============================================================================#
# BUILD_DIR is the location where all build artifacts are placed
BUILD_DIR := build
ROM            := $(BUILD_DIR)/$(TARGET_STRING).z64
ELF            := $(BUILD_DIR)/$(TARGET_STRING).elf
LD_SCRIPT      := polyline.ld
BOOT		:= /usr/lib/n64/PR/bootcode/boot.6102
BOOT_OBJ	:= $(BUILD_DIR)/boot.6102.o

# Directories containing source files
SRC_DIRS += src src/main src/buffers asm

C_FILES           := $(foreach dir,$(SRC_DIRS),$(wildcard $(dir)/*.c))
S_FILES           := $(foreach dir,$(SRC_DIRS),$(wildcard $(dir)/*.s))

# Object files
O_FILES := $(foreach file,$(C_FILES),$(BUILD_DIR)/$(file:.c=.o)) \
           $(foreach file,$(S_FILES),$(BUILD_DIR)/$(file:.s=.o)) \
		   $(BOOT_OBJ)

# Automatic dependency files
DEP_FILES := $(O_FILES:.o=.d) $(ASM_O_FILES:.o=.d)  $(BUILD_DIR)/$(LD_SCRIPT).d

#==============================================================================#
# Compiler Options                                                             #
#==============================================================================#

AS        := mips-n64-as
CC        := clang-12
CPP       := cpp
LD        := mips-n64-ld
AR        := mips-n64-ar
OBJDUMP   := mips-n64-objdump
OBJCOPY   := mips-n64-objcopy

INCLUDE_DIRS += /usr/include/n64 include $(BUILD_DIR) $(BUILD_DIR)/include src .

C_DEFINES := $(foreach d,$(DEFINES),-D$(d))
DEF_INC_CFLAGS := $(foreach i,$(INCLUDE_DIRS),-I$(i)) $(C_DEFINES)

CFLAGS = -G 0 -O2 -g -target mips -mabi=32 -fno-PIC -mno-abicalls -mips2 -ffreestanding $(DEF_INC_CFLAGS)
ASFLAGS     :=  -G 0 -O2 -g -mabi=32 -ffreestanding -mfix4300 $(DEF_INC_CFLAGS)

# C preprocessor flags
CPPFLAGS := -P -Wno-trigraphs $(DEF_INC_CFLAGS)

# tools
MAKESPHERE              := $(TOOLS_DIR)/makesphere
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

distclean: clean
	$(MAKE) -C $(TOOLS_DIR) clean

ALL_DIRS := $(BUILD_DIR) $(addprefix $(BUILD_DIR)/,$(SRC_DIRS))

# Make sure build directory exists before compiling anything
DUMMY != mkdir -p $(ALL_DIRS)

#==============================================================================#
# Compilation Recipes                                                          #
#==============================================================================#


$(BUILD_DIR)/src/main/static.o:	$(BUILD_DIR)/sphere.dat $(BUILD_DIR)/sphere.dl

$(BUILD_DIR)/sphere.dat: $(MAKESPHERE)
	@$(PRINT) "$(GREEN)Generating:  $(BLUE)$(BUILD_DIR)/sphere.dat $(NO_COL)\n"
	$(V)$(MAKESPHERE) > $(BUILD_DIR)/sphere.dat

$(BUILD_DIR)/sphere.dl: $(MAKESPHERE)
	@$(PRINT) "$(GREEN)Generating:  $(BLUE)$(BUILD_DIR)/sphere.dl $(NO_COL)\n"
	$(V)$(MAKESPHERE) -t > $(BUILD_DIR)/sphere.dl

# Compile C code
$(BUILD_DIR)/%.o: %.c
	$(call print,Compiling:,$<,$@)
	$(V)$(CC) -c $(CFLAGS) -D_LANGUAGE_C  -MMD -MF $(BUILD_DIR)/$*.d  -o $@ $<
$(BUILD_DIR)/%.o: $(BUILD_DIR)/%.c
	$(call print,Compiling:,$<,$@)
	$(V)$(CC) -c $(CFLAGS) -D_LANGUAGE_C  -MMD -MF $(BUILD_DIR)/$*.d  -o $@ $<

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

# Link final ELF file
$(ELF): $(O_FILES) $(BUILD_DIR)/$(LD_SCRIPT)
	@$(PRINT) "$(GREEN)Linking ELF file:  $(BLUE)$@ $(NO_COL)\n"
	$(V)$(LD) -L $(BUILD_DIR) -T $(BUILD_DIR)/$(LD_SCRIPT) -Map $(BUILD_DIR)/$(TARGET).map --no-check-sections -o $@ $(O_FILES) -L/usr/lib/n64 -lultra_rom -L$(N64_LIBGCCDIR) -lgcc

# Build ROM
$(ROM): $(ELF)
	$(call print,Building ROM:,$<,$@)
	$(V)$(OBJCOPY) --pad-to=0x100000 --gap-fill=0xFF $< $@ -O binary
	$(V)makemask $@

.PHONY: clean distclean default
# with no prerequisites, .SECONDARY causes no intermediate target to be removed
.SECONDARY:

# Remove built-in rules, to improve performance
MAKEFLAGS += --no-builtin-rules

-include $(DEP_FILES)

print-% : ; $(info $* is a $(flavor $*) variable set to [$($*)]) @true
