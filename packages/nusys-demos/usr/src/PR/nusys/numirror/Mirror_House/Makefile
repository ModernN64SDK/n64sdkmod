# Makefile to build mirror_house

TARGET_STRING := mirror_house
TARGET := $(TARGET_STRING)

# If SGISND is set to 0, SoundTools will be used
SGISND	= 0
ifeq	($(SGISND), 1)
AUDIO_CDEFS = NUALSGI=1 N_AUDIO=1      # Compile options for the SGI Sound Library version
NUAUDIOLIB = -lnualsgi -ln_audio      # The SGI Sound Library
else
AUDIO_CDEFS = NUALSTL=1 N_AUDIO=1   #Compile options for the N64 Sound Tools version
NUAUDIOLIB = -lnualstl_n -ln_mus -ln_audio_sc # N64 Sound Tools 
endif

# Preprocessor definitions
DEFINES := F3DEX_GBI_2=1 _WIN32=1 $(AUDIO_CDEFS) 

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
CODESEGMENT	   := $(BUILD_DIR)/codesegment.o
BOOT		:= /usr/lib/n64/PR/bootcode/boot.6102
BOOT_OBJ	:= $(BUILD_DIR)/boot.6102.o

# Directories containing source files
SRC_DIRS += src assets sound asm
CODE_SRC_DIRS := src/main

CODE_C_FILES           := $(foreach dir,$(CODE_SRC_DIRS),$(wildcard $(dir)/*.c))
LIBND_C_FILES     := $(wildcard src/nd/*.c)
LIBRESBG_C_FILES     := $(wildcard src/resbg/*.c)
LIBRESCHAR_C_FILES     := $(wildcard src/reschar/*.c)
C_FILES           := $(foreach dir,$(SRC_DIRS),$(wildcard $(dir)/*.c))
S_FILES           := $(foreach dir,$(SRC_DIRS),$(wildcard $(dir)/*.s))

CODE_O_FILES := /usr/lib/n64/nusys/nusys_rom.o $(foreach file,$(CODE_C_FILES),$(BUILD_DIR)/$(file:.c=.o))
LIBND_O_FILES := $(foreach file,$(LIBND_C_FILES),$(BUILD_DIR)/$(file:.c=.o))
LIBRESBG_O_FILES := $(foreach file,$(LIBRESBG_C_FILES),$(BUILD_DIR)/$(file:.c=.o))
LIBRESCHAR_O_FILES := $(foreach file,$(LIBRESCHAR_C_FILES),$(BUILD_DIR)/$(file:.c=.o))
# Object files
O_FILES := \
           $(foreach file,$(C_FILES),$(BUILD_DIR)/$(file:.c=.o)) \
           $(foreach file,$(S_FILES),$(BUILD_DIR)/$(file:.s=.o)) \
           $(CODESEGMENT) \
		   $(BOOT_OBJ)

# Automatic dependency files
DEP_FILES := $(O_FILES:.o=.d) $(ASM_O_FILES:.o=.d) $(CODE_O_FILES:.o=.d) $(BUILD_DIR)/$(LD_SCRIPT).d

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

INCLUDE_DIRS += /usr/include/n64 /usr/include/n64/PR \
				/usr/include/n64/nustd /usr/include/n64/nusys \
				include $(BUILD_DIR) $(BUILD_DIR)/include src .

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

ALL_DIRS := $(BUILD_DIR) $(addprefix $(BUILD_DIR)/,$(SRC_DIRS)) \
			 $(addprefix $(BUILD_DIR)/,$(CODE_SRC_DIRS)) \
			 $(BUILD_DIR)/src/resbg $(BUILD_DIR)/src/reschar \
			 $(BUILD_DIR)/src/nd

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

# Link libresbg
$(BUILD_DIR)/libresbg.a: $(LIBRESBG_O_FILES)
	@$(PRINT) "$(GREEN)Linking libresbg:  $(BLUE)$@ $(NO_COL)\n"
	$(V)$(AR) rcs -o $@ $(LIBRESBG_O_FILES)

# Link libreschar
$(BUILD_DIR)/libreschar.a: $(LIBRESCHAR_O_FILES)
	@$(PRINT) "$(GREEN)Linking libreschar:  $(BLUE)$@ $(NO_COL)\n"
	$(V)$(AR) rcs -o $@ $(LIBRESCHAR_O_FILES)

# Link libnd
$(BUILD_DIR)/libnd.a: $(LIBND_O_FILES)
	@$(PRINT) "$(GREEN)Linking libnd:  $(BLUE)$@ $(NO_COL)\n"
	$(V)$(AR) rcs -o $@ $(LIBND_O_FILES)

# Link codesegment
$(CODESEGMENT):	$(BUILD_DIR)/libresbg.a	$(BUILD_DIR)/libreschar.a $(BUILD_DIR)/libnd.a $(CODE_O_FILES)
	@$(PRINT) "$(GREEN)Linking codesegment:  $(BLUE)$@ $(NO_COL)\n"
	$(V)$(LD) -o $(CODESEGMENT) -r $(CODE_O_FILES) -L$(BUILD_DIR) -lreschar -lresbg -lnd -L/usr/lib/n64/nusys -L/usr/lib/n64 $(NUAUDIOLIB) -lnusys -lultra_rom -lnustd -L$(N64_LIBGCCDIR) -lgcc

# Link final ELF file
$(ELF): $(O_FILES) $(BUILD_DIR)/$(LD_SCRIPT)
	@$(PRINT) "$(GREEN)Linking ELF file:  $(BLUE)$@ $(NO_COL)\n"
	$(V)$(LD) -L $(BUILD_DIR) -T $(BUILD_DIR)/$(LD_SCRIPT) -Map $(BUILD_DIR)/$(TARGET).map --no-check-sections -o $@ $(O_FILES)

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
