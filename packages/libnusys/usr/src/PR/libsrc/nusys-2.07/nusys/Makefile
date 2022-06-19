# Makefile to build nusys

include util.mk

# Preprocessor definitions
DEFINES := USE_EPI=1

SRC_DIRS :=

# Whether to hide commands or not
VERBOSE ?= 0
ifeq ($(VERBOSE),0)
  V := @
endif

# Whether to colorize build messages
COLOR ?= 1

# VERSION 	   - selects the version of the library to build
#   libnusys	   - standard library
#   libnusys_d   - debug library
VERSION ?= libnusys
$(eval $(call validate-option,VERSION,libnusys libnusys_d))

ifeq      ($(VERSION),libnusys)
	OPT_FLAGS := -Os
else ifeq ($(VERSION),libnusys_d)
	OPT_FLAGS := -O0 -g
	DEFINES += NU_DEBUG=1
endif

TARGET := $(VERSION)

ifeq ($(filter clean,$(MAKECMDGOALS)),)
  $(info ==== Build Options ====)
  $(info Version:        $(VERSION))
  $(info =======================)
endif

#==============================================================================#
# Target Executable and Sources                                                #
#==============================================================================#

NUBOOTSRC		=	src/nuboot/nuboot.c
NUBOOTROMSRC	=	src/nuboot/nuboot_rom.c
NUBOOTPTNSRC	=	src/nuboot/nuboot_ptn.c
NUBOOTISVSRC	=	src/nuboot/nuboot_isv.c
NUBOOTEMUSRC	=	src/nuboot/nuboot_emu.c
NUBOOTMSPSRC	=	src/nuboot/nuboot_msp.c

NUBOOTOBJ		=	$(BUILD_DIR)/$(NUBOOTSRC:.c=.o)
NUBOOTROMOBJ	=	$(BUILD_DIR)/$(NUBOOTROMSRC:.c=.o)
NUBOOTPTNOBJ	=	$(BUILD_DIR)/$(NUBOOTPTNSRC:.c=.o)
NUBOOTISVOBJ	=	$(BUILD_DIR)/$(NUBOOTISVSRC:.c=.o)
NUBOOTEMUOBJ	=	$(BUILD_DIR)/$(NUBOOTEMUSRC:.c=.o)
NUBOOTMSPOBJ	=	$(BUILD_DIR)/$(NUBOOTMSPSRC:.c=.o)

NUBOOTLIB		= 	$(BUILD_DIR)/nusys.o
NUBOOTROMLIB	= 	$(BUILD_DIR)/nusys_rom.o
NUBOOTPTNLIB	= 	$(BUILD_DIR)/nusys_ptn.o
NUBOOTISVLIB	= 	$(BUILD_DIR)/nusys_isv.o
NUBOOTEMULIB	= 	$(BUILD_DIR)/nusys_emu.o
NUBOOTMSPLIB	= 	$(BUILD_DIR)/nusys_msp.o

NUBOOTDEBLIB	= 	$(BUILD_DIR)/nusys_d.o
NUBOOTPTNDEBLIB	= 	$(BUILD_DIR)/nusys_ptn_d.o
NUBOOTISVDEBLIB	= 	$(BUILD_DIR)/nusys_isv_d.o
NUBOOTEMUDEBLIB	= 	$(BUILD_DIR)/nusys_emu_d.o
NUBOOTMSPDEBLIB	= 	$(BUILD_DIR)/nusys_msp_d.o

ifeq      ($(VERSION),libnusys)
NUBOOTOBJS	=	$(NUBOOTLIB) \
			$(NUBOOTROMLIB) \
			$(NUBOOTPTNLIB) \
			$(NUBOOTISVLIB) \
			$(NUBOOTEMULIB) \
			$(NUBOOTMSPLIB)
else ifeq ($(VERSION),libnusys_d)
NUBOOTOBJS	=	$(NUBOOTDEBLIB) \
			$(NUBOOTPTNDEBLIB) \
			$(NUBOOTISVDEBLIB) \
			$(NUBOOTEMUDEBLIB) \
			$(NUBOOTMSPDEBLIB)
endif

BUILD_DIR_BASE := build
# BUILD_DIR is the location where all build artifacts are placed
BUILD_DIR      := $(BUILD_DIR_BASE)/$(VERSION)
LIB            := $(BUILD_DIR)/$(TARGET).a

# Directories containing source files
SRC_DIRS += src src/mainlib

NUBOOT_SRC_DIRS := src/nuboot

C_FILES           := $(foreach dir,$(SRC_DIRS),$(wildcard $(dir)/*.c))
NUBOOT_C_FILES    := $(foreach dir,$(NUBOOT_SRC_DIRS),$(wildcard $(dir)/*.c))
S_FILES           := $(foreach dir,$(SRC_DIRS),$(wildcard $(dir)/*.s))

# Object files
O_FILES := $(foreach file,$(C_FILES),$(BUILD_DIR)/$(file:.c=.o)) \
           $(foreach file,$(S_FILES),$(BUILD_DIR)/$(file:.s=.o))

NUBOOT_O_FILES := $(foreach file,$(NUBOOT_C_FILES),$(BUILD_DIR)/$(file:.c=.o))

# Automatic dependency files
DEP_FILES := $(O_FILES:.o=.d) $(NUBOOT_O_FILES:.o=.d) $(ASM_O_FILES:.o=.d)

#==============================================================================#
# Compiler Options                                                             #
#==============================================================================#

AS        := mips-n64-as
CC        := mips-n64-gcc
CPP       := cpp
LD        := mips-n64-ld
AR        := mips-n64-ar

INCLUDE_DIRS += /usr/include/n64 /usr/include/n64/PR /usr/include/n64/nusys include $(BUILD_DIR) $(BUILD_DIR)/include src .

C_DEFINES := $(foreach d,$(DEFINES),-D$(d))
DEF_INC_CFLAGS := $(foreach i,$(INCLUDE_DIRS),-I$(i)) $(C_DEFINES)

CFLAGS = -G 0 $(OPT_FLAGS) -mabi=32 -ffreestanding -mfix4300 $(DEF_INC_CFLAGS) -Wall -fwrapv
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
default: $(LIB)

clean:
	$(RM) -r $(BUILD_DIR_BASE)

ALL_DIRS := $(BUILD_DIR) $(addprefix $(BUILD_DIR)/,$(SRC_DIRS)) $(addprefix $(BUILD_DIR)/,$(NUBOOT_SRC_DIRS))

# Make sure build directory exists before compiling anything
DUMMY != mkdir -p $(ALL_DIRS)

#==============================================================================#
# Compilation Recipes                                                          #
#==============================================================================#

$(NUBOOTLIB): $(NUBOOTOBJ)
	$(call print,Linking:,$<,$@)
	$(V)$(LD) -o $(NUBOOTLIB) -r $(NUBOOTOBJ)
$(NUBOOTROMLIB): $(NUBOOTOBJ) 
	$(call print,Linking:,$<,$@)
	$(V)$(LD) -o $(NUBOOTROMLIB) -r $(NUBOOTOBJ)
$(NUBOOTPTNLIB): $(NUBOOTROMOBJ) 
	$(call print,Linking:,$<,$@)
	$(V)$(LD) -o $(NUBOOTPTNLIB) -r $(NUBOOTROMOBJ)
$(NUBOOTISVLIB): $(NUBOOTISVOBJ) 
	$(call print,Linking:,$<,$@)
	$(V)$(LD) -o $(NUBOOTISVLIB) -r $(NUBOOTISVOBJ)
$(NUBOOTEMULIB): $(NUBOOTEMUOBJ) 
	$(call print,Linking:,$<,$@)
	$(V)$(LD) -o $(NUBOOTEMULIB) -r $(NUBOOTEMUOBJ)
$(NUBOOTMSPLIB): $(NUBOOTMSPOBJ) 
	$(call print,Linking:,$<,$@)
	$(V)$(LD) -o $(NUBOOTMSPLIB) -r $(NUBOOTMSPOBJ)
$(NUBOOTDEBLIB): $(NUBOOTOBJ) 
	$(call print,Linking:,$<,$@)
	$(V)$(LD) -o $(NUBOOTDEBLIB) -r $(NUBOOTOBJ)
$(NUBOOTPTNDEBLIB): $(NUBOOTPTNOBJ) 
	$(call print,Linking:,$<,$@)
	$(V)$(LD) -o $(NUBOOTPTNDEBLIB) -r $(NUBOOTPTNOBJ)
$(NUBOOTISVDEBLIB): $(NUBOOTISVOBJ) 
	$(call print,Linking:,$<,$@)
	$(V)$(LD) -o $(NUBOOTISVDEBLIB) -r $(NUBOOTISVOBJ)
$(NUBOOTEMUDEBLIB): $(NUBOOTEMUOBJ) 
	$(call print,Linking:,$<,$@)
	$(V)$(LD) -o $(NUBOOTEMUDEBLIB) -r $(NUBOOTEMUOBJ)
$(NUBOOTMSPDEBLIB): $(NUBOOTMSPOBJ) 
	$(call print,Linking:,$<,$@)
	$(V)$(LD) -o $(NUBOOTMSPDEBLIB) -r $(NUBOOTMSPOBJ)

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

# Link final ELF file
$(LIB): $(NUBOOTOBJS) $(O_FILES)
	@$(PRINT) "$(GREEN)Linking $(VERSION):  $(BLUE)$@ $(NO_COL)\n"
	$(V)$(AR) rcs -o $@ $(O_FILES)

all:
	$(MAKE) VERSION=libnusys
	$(MAKE) VERSION=libnusys_d

.PHONY: clean default
# with no prerequisites, .SECONDARY causes no intermediate target to be removed
.SECONDARY:

# Remove built-in rules, to improve performance
MAKEFLAGS += --no-builtin-rules

-include $(DEP_FILES)

print-% : ; $(info $* is a $(flavor $*) variable set to [$($*)]) @true
