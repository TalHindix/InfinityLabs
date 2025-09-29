# make/mt_counting_sort.mak

# Compiler
CC := gcc

# Source paths
SORTING_SRC_DIR := sorting/src
SORTING_INC_DIR := sorting/inc
SORTING_TEST_DIR := sorting/test

# Include flags
INC_DIRS := -I$(SORTING_INC_DIR)

# Build type from CFLAGS
ifeq ($(findstring -g,$(CFLAGS)),-g)
    BUILD_DIR := bin/debug
else
    BUILD_DIR := bin/release
endif

# Output binary
.DEFAULT_GOAL := $(BUILD_DIR)/mt_counting_sort_test.out
TARGET := $(BUILD_DIR)/mt_counting_sort_test.out

# Sources and objects
SRC := $(SORTING_SRC_DIR)/linear_sorting.c $(SORTING_SRC_DIR)/mt_counting_sort.c $(SORTING_TEST_DIR)/mt_counting_sort_test.c
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
	$(CC) $(CFLAGS) $(INC_DIRS) -o $@ $^ -lpthread

# Dependency includes
-include $(OBJS:.o=.d)