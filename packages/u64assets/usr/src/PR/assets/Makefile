# Makefile to build audio assets

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
BUILD_DIR	:= build

# Directories containing source files
SRC_DIRS	:= sounds sfx banks sequences

# Sound files
AIFF_FILES	:= $(foreach dir,$(SRC_DIRS),$(wildcard $(dir)/*.aiff))

AIFC_FILES	:= $(foreach file,$(AIFF_FILES),$(BUILD_DIR)/$(file:.aiff=.aifc))

# Instrument banks
INST_FILES	:= $(foreach dir,$(SRC_DIRS),$(wildcard $(dir)/*.inst))

CTL_FILES	:= $(foreach file,$(INST_FILES),$(BUILD_DIR)/$(file:.inst=.ctl))
TBL_FILES	:= $(foreach file,$(INST_FILES),$(BUILD_DIR)/$(file:.inst=.tbl))

# MIDI

MIDI_FILES	:= $(foreach dir,$(SRC_DIRS),$(wildcard $(dir)/*.mid))

SEQ_FILES	:= $(foreach file,$(MIDI_FILES),$(BUILD_DIR)/$(file:.mid=.seq))
CMF_FILES	:= $(foreach file,$(MIDI_FILES),$(BUILD_DIR)/$(file:.mid=.cmf))

SEQCOLLECTION	:= $(BUILD_DIR)/ship.sbk

CMPCOLLECTION	:= $(BUILD_DIR)/cmpship.sbk

SOUNDDATA	:= $(AIFC_FILES) $(TBL_FILES) $(SEQCOLLECTION) $(CMPCOLLECTION)

#==============================================================================#
# Compiler Options                                                             #
#==============================================================================#

TABLEDESIGN	:= tabledesign
VADPCM_ENC	:= vadpcm_enc
IC			:= ic
MIDICVT		:= midicvt
MIDICOMP	:= midicomp
SBC			:= sbc

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
default: $(SOUNDDATA)

clean:
	$(RM) -r $(BUILD_DIR)

ALL_DIRS := $(BUILD_DIR) $(addprefix $(BUILD_DIR)/,$(SRC_DIRS))

# Make sure build directory exists before compiling anything
DUMMY != mkdir -p $(ALL_DIRS)

#==============================================================================#
# Compilation Recipes                                                          #
#==============================================================================#

$(BUILD_DIR)/%.table: %.aiff
	$(call print,Generating codebook:,$<,$@)
	$(V)$(TABLEDESIGN) $< > $@

$(BUILD_DIR)/%.aifc: $(BUILD_DIR)/%.table %.aiff
	$(call print,Encoding ADPCM:,$(word 2,$^),$@)
	$(V)$(VADPCM_ENC) -c $^ $@

$(BUILD_DIR)/%.ctl: %.inst $(AIFC_FILES)
	$(call print,Building soundbank:,$<,$@)
	$(V)$(IC) -o $(basename $@) $<

$(BUILD_DIR)/%.tbl: $(BUILD_DIR)/%.ctl
	@true

$(BUILD_DIR)/%.seq: %.mid
	$(call print,Converting MIDI to SEQ:,$<,$@)
	$(V)$(MIDICVT) -s $< $@

$(BUILD_DIR)/%.cmf: %.mid
	$(call print,Compressing MIDI to CMF:,$<,$@)
	$(V)$(MIDICOMP) $< $@

$(CMPCOLLECTION): $(CMF_FILES)
	$(call print,Building sequence bank:,,$@)
	$(V)$(SBC) -o $@ $(CMF_FILES)

$(SEQCOLLECTION): $(SEQ_FILES)
	$(call print,Building sequence bank:,,$@)
	$(V)$(SBC) -o $@ $(SEQ_FILES)
	
.PHONY: clean default
# with no prerequisites, .SECONDARY causes no intermediate target to be removed
.SECONDARY:

# Remove built-in rules, to improve performance
MAKEFLAGS += --no-builtin-rules

print-% : ; $(info $* is a $(flavor $*) variable set to [$($*)]) @true
