# make/fsa.mak

# Compiler
CC := gcc

# Source paths
FSA_SRC_DIR := fsa/src
FSA_INC_DIR := fsa/inc
FSA_TEST_DIR := fsa/test

# Include flags
INC_DIRS := -I$(FSA_INC_DIR)

# Build type from CFLAGS
ifeq ($(findstring -g,$(CFLAGS)),-g)
    BUILD_DIR := bin/debug
else
    BUILD_DIR := bin/release
endif

# Output binary
.DEFAULT_GOAL := $(BUILD_DIR)/fsa_test.out
TARGET := $(BUILD_DIR)/fsa_test.out

# Sources and objects
SRC := $(FSA_SRC_DIR)/fsa.c $(FSA_TEST_DIR)/fsa_test.c
OBJS := $(SRC:.c=.o)

# Rules

# Create build dir
$(BUILD_DIR):
	mkdir -p $@

# Build object files
%.o: %.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $(INC_DIRS) -c -o $@ $<

# Build final executable
$(TARGET): $(OBJS) | $(BUILD_DIR)
	$(CC) $(CFLAGS) $(INC_DIRS) -o $@ $^

# Dependency includes
-include $(OBJS:.o=.d)

