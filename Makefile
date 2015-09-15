# {{{ Configuration. ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

# The type of thing to be built (static or shared or executable).
# Be sure to NOT use quotes.
BUILD_TYPE ?= static

# The name of the executable to be created.
NAME ?= $(notdir $(CURDIR))
# Archiver used.
AR ?= ar
# Compiler used.
CC ?= gcc
# Path to the source directory, relative to the Makefile.
SRC_PATH = ./src
# Space-separated pkg-config libraries used by this project.
LIBS =
# General compiler flags.
COMPILE_FLAGS = -std=c99 -Wall -Wextra -g
# Additional release-specific flags.
RCOMPILE_FLAGS = -D NDEBUG
# Additional debug-specific flags.
DCOMPILE_FLAGS = -D DEBUG -ggdb3
# Add additional include paths.
INCLUDES = -I $(SRC_PATH)/
# General linker settings.
LINK_FLAGS =
# Additional release-specific linker settings.
RLINK_FLAGS =
# Additional debug-specific linker settings.
DLINK_FLAGS =
# Destination directory, like a jail or mounted system.
DESTDIR = /
# Install path (bin/ or lib/ is appended automatically).
INSTALL_PREFIX = usr/local

# The extension for source files.
SRC_EXT = c
# The extension for executable files (e.g. 'elf').
EXECUTABLE_EXT = elf
# The extension for shared libraries (i.e. dynamic libraries) (e.g. 'so').
SHARED_LIB_EXT = so
# The extension for static libraries (e.g. 'a').
STATIC_LIB_EXT = a

# Shell used in this Makefile.
SHELL = /bin/bash

# Programs for installation.
INSTALL = install
INSTALL_PROGRAM = $(INSTALL)
INSTALL_DATA = $(INSTALL) -m 644

# }}} ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

# {{{ Post-configuration. ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

# Set the output file extension (the generated artifact).
ifeq ($(BUILD_TYPE),executable)
	OUT_EXT = $(EXECUTABLE_EXT)
	INSTALL_DIR = $(INSTALL_PREFIX)/bin
	INSTALL_FILE = $(NAME)
else ifeq ($(BUILD_TYPE),shared)
	OUT_EXT = $(SHARED_LIB_EXT)
	INSTALL_DIR = $(INSTALL_PREFIX)/lib
	INSTALL_FILE = $(NAME).$(OUT_EXT)
	LINK_FLAGS += -shared
else ifeq ($(BUILD_TYPE),static)
	OUT_EXT = $(STATIC_LIB_EXT)
	INSTALL_DIR = $(INSTALL_PREFIX)/lib
	INSTALL_FILE = $(NAME).$(OUT_EXT)
else
$(error Invalid build type.)
endif

# Append pkg-config specific libraries if need be.
ifneq ($(LIBS),)
	COMPILE_FLAGS += $(shell pkg-config --cflags $(LIBS))
	LINK_FLAGS += $(shell pkg-config --libs $(LIBS))
endif

# Verbose option, to output compile and link commands.
export V := false
export CMD_PREFIX := @
ifeq ($(V),true)
	CMD_PREFIX :=
endif

# Combine compiler and linker flags.
release: export CFLAGS := $(CFLAGS) $(COMPILE_FLAGS) $(RCOMPILE_FLAGS)
release: export LDFLAGS := $(LDFLAGS) $(LINK_FLAGS) $(RLINK_FLAGS)
debug: export CFLAGS := $(CFLAGS) $(COMPILE_FLAGS) $(DCOMPILE_FLAGS)
debug: export LDFLAGS := $(LDFLAGS) $(LINK_FLAGS) $(DLINK_FLAGS)

# Build and output paths.
release: export BUILD_PATH := build/release
release: export BIN_PATH := bin/release
debug: export BUILD_PATH := build/debug
debug: export BIN_PATH := bin/debug
install: export BIN_PATH := bin/release

# Find all source files in the source directory, sorted by most recently
# modified.
SOURCES = $(shell find $(SRC_PATH)/ -name '*.$(SRC_EXT)' -printf '%T@\t%p\n' \
					| sort -k 1nr | cut -f2-)
# Fallback in case the above fails.
rwildcard = $(foreach d, $(wildcard $1*), $(call rwildcard,$d/,$2) \
						$(filter $(subst *,%,$2), $d))
ifeq ($(SOURCES),)
	SOURCES := $(call rwildcard, $(SRC_PATH)/, *.$(SRC_EXT))
endif

# Set the object file names, with the source directory stripped
# from the path, and the build path prepended in its place.
OBJECTS = $(SOURCES:$(SRC_PATH)/%.$(SRC_EXT)=$(BUILD_PATH)/%.o)

# Set the dependency files that will be used to add header dependencies.
DEPS = $(OBJECTS:.o=.d)

# }}} ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

# {{{ Macros - Timing compilation. ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

TIME_FILE = $(dir $@).$(notdir $@)_time
START_TIME = date '+%s' > $(TIME_FILE)
END_TIME = read st < $(TIME_FILE) ; \
	$(RM) $(TIME_FILE) ; \
	st=$$((`date '+%s'` - $$st - 86400)) ; \
	echo `date -u -d @$$st '+%H:%M:%S'`

# }}} ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

# {{{ Functions. ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

# Function used to check variables.
# Useful for debugging and adding features.
#
# Use on the command line:
# 		make print-VARNAME
print-%: ; @echo $*=$($*)

# }}} ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

# {{{ Tasks. ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

# Clear built-in rules.
.SUFFIXES:

# {{{ Setup directories.  ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
.PHONY: dirs
dirs:
	@echo "Creating directories."
	@mkdir -p $(dir $(OBJECTS))
	@mkdir -p $(BIN_PATH)
# }}} ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

# {{{ Clean. ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
.PHONY: clean
clean:
	@echo "Deleting 'build'."
	@$(RM) -r build
	@echo "Deleting 'bin'."
	@$(RM) -r bin
# }}} ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

# {{{ Build - Release. ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
.PHONY: release
release: dirs
	@echo "Beginning release build."
	@$(START_TIME)
	@$(MAKE) all --no-print-directory
	@echo -n "Total build time: "
	@$(END_TIME)
# }}} ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

# {{{ Build - Debug. ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
.PHONY: debug
debug: dirs
	@echo "Beginning debug build."
	@$(START_TIME)
	@$(MAKE) all --no-print-directory
	@echo -n "Total build time: "
	@$(END_TIME)
# }}} ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

# {{{ Install. ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
.PHONY: install
install:
	@echo "Installing to '$(DESTDIR)$(INSTALL_DIR)/$(INSTALL_FILE)'."
	@$(INSTALL_PROGRAM) $(BIN_PATH)/$(NAME).$(OUT_EXT) \
											$(DESTDIR)$(INSTALL_DIR)/$(INSTALL_FILE)
# }}} ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

# {{{ Uninstall. ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
.PHONY: uninstall
uninstall:
	@echo "Removing '$(DESTDIR)$(INSTALL_DIR)/$(NAME)'."
	@$(RM) $(DESTDIR)$(INSTALL_DIR)/$(INSTALL_FILE)
# }}} ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

# {{{ Main rule: build. ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
all: $(BIN_PATH)/$(NAME).$(OUT_EXT)
# }}} ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

# }}} ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

# {{{ File rules. ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

# {{{ Create the output artifact (link objects). ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
$(BIN_PATH)/$(NAME).$(OUT_EXT): $(OBJECTS)
	@echo "Linking: '$@'."
	@$(START_TIME)
ifeq ($(BUILD_TYPE),static)
	$(CMD_PREFIX)$(AR) rcs $@ $(OBJECTS)
else
	$(CMD_PREFIX)$(CC) $(OBJECTS) $(LDFLAGS) -o $@
endif
	@echo -en "\t Link time: "
	@$(END_TIME)
# }}} ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

# Add dependency files, if they exist.
-include $(DEPS)

# {{{ Create the object files. ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
# After the first compilation they will be joined with the rules from the
# dependency files to provide header dependencies.
$(BUILD_PATH)/%.o: $(SRC_PATH)/%.$(SRC_EXT)
	@echo "Compiling: '$<' → '$@'."
	@$(START_TIME)
	$(CMD_PREFIX)$(CC) $(CFLAGS) $(INCLUDES) -MP -MMD -c $< -o $@
	@echo -en "\t Compile time: "
	@$(END_TIME)
# }}} ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

# }}} ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
