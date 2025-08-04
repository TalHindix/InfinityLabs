DS_NAME = sll

INC_DIR = inc
SRC_DIR = src
TEST_DIR = test

# Get the action and determine the output directory
ifeq ($(findstring -g,$(CFLAGS)),-g)
OUTPUT_DIR = bin/debug
else
OUTPUT_DIR = bin/release
endif

TARGET = $(OUTPUT_DIR)/$(DS_NAME)_test.out
LIB = $(OUTPUT_DIR)/lib$(DS_NAME).so

# Build Shared library #
$(LIB): $(SRC_DIR)/$(DS_NAME).c $(INC_DIR)/$(DS_NAME).h
	$(CC) $(CFLAGS) -I$(INC_DIR) -shared -o $@ $<

# Build Test object file #
$(OUTPUT_DIR)/$(DS_NAME)_test.o: $(TEST_DIR)/$(DS_NAME)_test.c
	$(CC) $(CFLAGS) -I$(INC_DIR) -c -o $@ $<

# Test executable linked with shared library #
$(TARGET): $(OUTPUT_DIR)/$(DS_NAME)_test.o $(LIB)
	$(CC) $(CFLAGS) -o $@ $< -L$(OUTPUT_DIR) -l$(DS_NAME) -Wl,-rpath=$(OUTPUT_DIR)

# The default goal is now the executable in the correct bin subdirectory.
.DEFAULT_GOAL := $(TARGET)

# Include dependency files for automatic recompilation
-include $(patsubst $(SRC_DIR)/%.c,$(OUTPUT_DIR)/%.d,$(wildcard $(SRC_DIR)/*.c))
-include $(patsubst $(TEST_DIR)/%.c,$(OUTPUT_DIR)/%.d,$(wildcard $(TEST_DIR)/*.c))
