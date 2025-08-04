DS_NAME = sortedl
DEP_DS_NAME = dll

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
DEP_LIB = $(OUTPUT_DIR)/lib$(DEP_DS_NAME).so

# Build Dependency Shared library (libdll.so) #
$(DEP_LIB): $(SRC_DIR)/$(DEP_DS_NAME).c $(INC_DIR)/$(DEP_DS_NAME).h
	$(CC) $(CFLAGS) -I$(INC_DIR) -shared -o $@ $<

# Build Main Shared library (libsortedl.so) #
$(LIB): $(SRC_DIR)/$(DS_NAME).c $(INC_DIR)/$(DS_NAME).h $(INC_DIR)/$(DEP_DS_NAME).h
	$(CC) $(CFLAGS) -I$(INC_DIR) -shared -o $@ $<

# Build Test object file #
$(OUTPUT_DIR)/$(DS_NAME)_test.o: $(TEST_DIR)/$(DS_NAME)_test.c $(INC_DIR)/$(DEP_DS_NAME).h
	$(CC) $(CFLAGS) -I$(INC_DIR) -c -o $@ $<

# Test executable linked with both shared libraries #
$(TARGET): $(OUTPUT_DIR)/$(DS_NAME)_test.o $(LIB) $(DEP_LIB)
	$(CC) $(CFLAGS) -o $@ $< -L$(OUTPUT_DIR) -l$(DS_NAME) -l$(DEP_DS_NAME) -Wl,-rpath=$(OUTPUT_DIR)

# The default goal is now the executable in the correct bin subdirectory.
.DEFAULT_GOAL := $(TARGET)

# Include dependency files for automatic recompilation
-include $(patsubst $(SRC_DIR)/%.c,$(OUTPUT_DIR)/%.d,$(wildcard $(SRC_DIR)/*.c))
-include $(patsubst $(TEST_DIR)/%.c,$(OUTPUT_DIR)/%.d,$(wildcard $(TEST_DIR)/*.c))
