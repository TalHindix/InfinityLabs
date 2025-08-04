DS_NAME = pq
DEP1_DS_NAME = sortedl
DEP2_DS_NAME = dll

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
DEP1_LIB = $(OUTPUT_DIR)/lib$(DEP1_DS_NAME).so
DEP2_LIB = $(OUTPUT_DIR)/lib$(DEP2_DS_NAME).so

# Build Dependency 2 Shared library (libdll.so) #
$(DEP2_LIB): $(SRC_DIR)/$(DEP2_DS_NAME).c $(INC_DIR)/$(DEP2_DS_NAME).h
	$(CC) $(CFLAGS) -I$(INC_DIR) -shared -o $@ $<

# Build Dependency 1 Shared library (libsortedl.so) #
$(DEP1_LIB): $(SRC_DIR)/$(DEP1_DS_NAME).c $(INC_DIR)/$(DEP1_DS_NAME).h $(INC_DIR)/$(DEP2_DS_NAME).h | $(DEP2_LIB)
	$(CC) $(CFLAGS) -I$(INC_DIR) -shared -o $@ $<

# Build Main Shared library (libpq.so) #
$(LIB): $(SRC_DIR)/$(DS_NAME).c $(INC_DIR)/$(DS_NAME).h $(INC_DIR)/$(DEP1_DS_NAME).h | $(DEP1_LIB)
	$(CC) $(CFLAGS) -I$(INC_DIR) -shared -o $@ $<

# Build Test object file #
$(OUTPUT_DIR)/$(DS_NAME)_test.o: $(TEST_DIR)/$(DS_NAME)_test.c $(INC_DIR)/$(DS_NAME).h $(INC_DIR)/$(DEP1_DS_NAME).h $(INC_DIR)/$(DEP2_DS_NAME).h
	$(CC) $(CFLAGS) -I$(INC_DIR) -c -o $@ $<

# Test executable linked with all shared libraries #
$(TARGET): $(OUTPUT_DIR)/$(DS_NAME)_test.o $(LIB) $(DEP1_LIB) $(DEP2_LIB)
	$(CC) $(CFLAGS) -o $@ $< -L$(OUTPUT_DIR) -l$(DS_NAME) -l$(DEP1_DS_NAME) -l$(DEP2_DS_NAME) -Wl,-rpath=$(OUTPUT_DIR)

# The default goal is now the executable in the correct bin subdirectory.
.DEFAULT_GOAL := $(TARGET)

# Include dependency files for automatic recompilation
-include $(patsubst $(SRC_DIR)/%.c,$(OUTPUT_DIR)/%.d,$(wildcard $(SRC_DIR)/*.c))
-include $(patsubst $(TEST_DIR)/%.c,$(OUTPUT_DIR)/%.d,$(wildcard $(TEST_DIR)/*.c))
