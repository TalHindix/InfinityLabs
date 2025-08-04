# make/vsa.mak

# Compiler
CC := gcc

# Paths
VSA_SRC_DIR := vsa/src
VSA_INC_DIR := vsa/inc
VSA_TEST_DIR := vsa/test

# Include flags
INC_DIRS := -I$(VSA_INC_DIR)

# Determine build dir based on CFLAGS
ifeq ($(findstring -g,$(CFLAGS)),-g)
    BUILD_DIR := bin/debug
else
    BUILD_DIR := bin/release
endif

# Output file
.DEFAULT_GOAL := $(BUILD_DIR)/vsa_test.out
TARGET := $(BUILD_DIR)/vsa_test.out

# Sources and object files
SRC := $(VSA_SRC_DIR)/vsa.c $(VSA_TEST_DIR)/vsa_test.c
OBJS := $(SRC:.c=.o)

# Ensure build dir exists
$(BUILD_DIR):
	mkdir -p $@

# Compile objects
%.o: %.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $(INC_DIRS) -c $< -o $@

# Link final binary
$(TARGET): $(OBJS) | $(BUILD_DIR)
	$(CC) $(CFLAGS) $(INC_DIRS) -o $@ $^

# Auto-dependencies
-include $(OBJS:.o=.d)

# Optional clean
.PHONY: clean
clean:
	rm -f $(OBJS) $(TARGET)

