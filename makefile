QUIET = @

# tools
CC = $(ARM_SDK_PREFIX)gcc
CP = $(ARM_SDK_PREFIX)objcopy
ECHO = echo

# common variables
IDENTIFIER := AM32

# Folders
HAL_FOLDER := Mcu
MAIN_SRC_DIR := Src
MAIN_INC_DIR := Inc

SRC_DIRS_COMMON := $(MAIN_SRC_DIR)

# Include processor specific makefiles
include f051makefile.mk
include g071makefile.mk

# Default MCU type to F051
MCU_TYPE ?= F051

# additional libs
LIBS := -lc -lm -lnosys

# Compiler options
CFLAGS_COMMON := -DUSE_MAKE
CFLAGS_COMMON += -I$(MAIN_INC_DIR) -O2 -Wall -fdata-sections -ffunction-sections
CFLAGS_COMMON += -D$(TARGET)

# Linker options
LDFLAGS_COMMON := -specs=nano.specs $(LIBS) -Wl,--gc-sections -Wl,--print-memory-usage

# Working directories
ROOT := $(patsubst %/,%,$(dir $(lastword $(MAKEFILE_LIST))))

# Search source files
SRC_COMMON := $(foreach dir,$(SRC_DIRS_COMMON),$(wildcard $(dir)/*.[cs]))

VERSION_MAJOR := $(shell grep " VERSION_MAJOR" $(MAIN_SRC_DIR)/main.c | awk '{print $$3}' )
VERSION_MINOR := $(shell grep " VERSION_MINOR" $(MAIN_SRC_DIR)/main.c | awk '{print $$3}' )

FIRMWARE_VERSION := $(VERSION_MAJOR).$(VERSION_MINOR)

TARGET_BASENAME = $(BIN_DIR)/$(IDENTIFIER)_$(TARGET)_$(FIRMWARE_VERSION)

# Build tools, so we all share the same versions
# import macros common to all supported build systems
include $(ROOT)/make/system-id.mk

# configure some directories that are relative to wherever ROOT_DIR is located
BIN_DIR := $(ROOT)/obj

TOOLS_DIR ?= $(ROOT)/tools
DL_DIR := $(ROOT)/downloads

.PHONY : clean all binary f051 g071 
all : $(TARGETS_F051) $(TARGETS_G071) 
f051 : $(TARGETS_F051)
g071 : $(TARGETS_G071)

clean :
	rm -rf $(BIN_DIR)/*

binary : $(TARGET_BASENAME).bin
	@$(ECHO) All done

$(TARGETS_F051) :
	@$(MAKE) -s MCU_TYPE=F051 TARGET=$@ binary

$(TARGETS_G071) :
	@$(MAKE) -s MCU_TYPE=G071 TARGET=$@ binary



# Compile target
$(TARGET_BASENAME).elf: SRC := $(SRC_COMMON) $(SRC_$(MCU_TYPE))
$(TARGET_BASENAME).elf: CFLAGS := $(MCU_F051) $(CFLAGS_$(MCU_TYPE)) $(CFLAGS_COMMON)
$(TARGET_BASENAME).elf: LDFLAGS := $(LDFLAGS_COMMON) $(LDFLAGS_$(MCU_TYPE)) -T$(LDSCRIPT_$(MCU_TYPE))
$(TARGET_BASENAME).elf: $(SRC)
	@$(ECHO) Compiling $(notdir $@)
	$(QUIET)mkdir -p $(dir $@)
	$(QUIET)$(CC) $(CFLAGS) $(LDFLAGS) -MMD -MP -MF $(@:.elf=.d) -o $(@) $(SRC)

# Generate bin and hex files
$(TARGET_BASENAME).bin: $(TARGET_BASENAME).elf
	@$(ECHO) Generating $(notdir $@)
	$(QUIET)$(CP) -O binary $(<) $@
	$(QUIET)$(CP) $(<) -O ihex $(@:.bin=.hex)

# mkdirs
$(DL_DIR):
	$(QUIET)mkdir -p $@

$(TOOLS_DIR):
	$(QUIET)mkdir -p $@

# include the tools makefile
include $(ROOT)/make/tools.mk
