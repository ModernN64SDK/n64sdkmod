# Makefile to build libmus

include util.mk

# Preprocessor definitions
DEFINES := 

SRC_DIRS :=

# Whether to hide commands or not
VERBOSE ?= 0
ifeq ($(VERBOSE),0)
  V := @
endif

# Whether to colorize build messages
COLOR ?= 1

# VERSION 	   - selects the version of the library to build
#   libmus	   - standard library
#   libn_mus   - n_audio library
#   libmus_d   - debug library
#   libn_mus_d - debug n_audio library
VERSION ?= libmus
$(eval $(call validate-option,VERSION,libmus libn_mus libmus_d libn_mus_d))

ifeq      ($(VERSION),libmus)
	OPT_FLAGS := -O3
else ifeq ($(VERSION),libn_mus)
	OPT_FLAGS := -O3
	DEFINES += SUPPORT_NAUDIO=1
else ifeq ($(VERSION),libmus_d)
	OPT_FLAGS := -O0 -g
	DEFINES += _AUDIODEBUG=1
else ifeq ($(VERSION),libn_mus_d)
	OPT_FLAGS := -O0 -g
	DEFINES += _AUDIODEBUG=1
	DEFINES += SUPPORT_NAUDIO=1
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
BUILD_DIR_BASE := build
# BUILD_DIR is the location where all build artifacts are placed
BUILD_DIR      := $(BUILD_DIR_BASE)/$(VERSION)
LIB            := $(BUILD_DIR)/$(TARGET).a

# Directories containing source files
SRC_DIRS += src

C_FILES           := $(foreach dir,$(SRC_DIRS),$(wildcard $(dir)/*.c))
S_FILES           := $(foreach dir,$(SRC_DIRS),$(wildcard $(dir)/*.s))

# Object files
O_FILES := $(foreach file,$(C_FILES),$(BUILD_DIR)/$(file:.c=.o)) \
           $(foreach file,$(S_FILES),$(BUILD_DIR)/$(file:.s=.o))

# Automatic dependency files
DEP_FILES := $(O_FILES:.o=.d) $(ASM_O_FILES:.o=.d)

#==============================================================================#
# Compiler Options                                                             #
#==============================================================================#

AS        := mips-n64-as
CC        := mips-n64-gcc
CPP       := cpp
LD        := mips-n64-ld
AR        := mips-n64-ar

INCLUDE_DIRS += /usr/include/n64 /usr/include/n64/PR include $(BUILD_DIR) $(BUILD_DIR)/include src .

C_DEFINES := $(foreach d,$(DEFINES),-D$(d))
DEF_INC_CFLAGS := $(foreach i,$(INCLUDE_DIRS),-I$(i)) $(C_DEFINES)

CFLAGS = -G 0 $(OPT_FLAGS) -mabi=32 -ffreestanding -mfix4300 $(DEF_INC_CFLAGS) -Wall
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

ALL_DIRS := $(BUILD_DIR) $(addprefix $(BUILD_DIR)/,$(SRC_DIRS))

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

# Link final ELF file
$(LIB): $(O_FILES)
	@$(PRINT) "$(GREEN)Linking $(VERSION):  $(BLUE)$@ $(NO_COL)\n"
	$(V)$(AR) rcs -o $@ $(O_FILES)

all:
	$(MAKE) VERSION=libmus
	$(MAKE) VERSION=libmus_d
	$(MAKE) VERSION=libn_mus
	$(MAKE) VERSION=libn_mus_d

.PHONY: clean default
# with no prerequisites, .SECONDARY causes no intermediate target to be removed
.SECONDARY:

# Remove built-in rules, to improve performance
MAKEFLAGS += --no-builtin-rules

-include $(DEP_FILES)

print-% : ; $(info $* is a $(flavor $*) variable set to [$($*)]) @true
