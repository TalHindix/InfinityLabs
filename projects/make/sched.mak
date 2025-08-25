# Determine build type based on CFLAGS #
ifeq ($(findstring -g,$(CFLAGS)),-g)
    BUILD_DIR = bin/debug
else
    BUILD_DIR = bin/release
endif

.DEFAULT_GOAL := $(BUILD_DIR)/sched_test.out

INC_DIRS = -I$(SCHED_INC_DIR) -I$(DS_INC_DIR) 

# APIs to turn into .so files #
LIB_NAMES = pq uid heap vector
SCHED_NAMES = task sched

# Source directories #
DS_INC_DIR = ../ds/inc
DS_SRC_DIR = ../ds/src
SCHED_SRC_DIR = sched/src
SCHED_INC_DIR = sched/inc
TEST_DIR = sched/test

# .so file names #
DS_SO_FILES = $(addprefix $(DS_SRC_DIR)/lib, $(addsuffix .so, $(LIB_NAMES)))
SCHED_SO_FILE = $(addprefix $(SCHED_SRC_DIR)/lib, $(addsuffix .so,$(SCHED_NAMES)))

TARGET = $(BUILD_DIR)/sched_test.out
OBJS = $(TEST_DIR)/sched_test.o

# Create build directory #
$(BUILD_DIR):
	mkdir -p $@

# Build Shared librarys files in ../ds/src/ #
$(DS_SRC_DIR)/lib%.so: $(DS_SRC_DIR)/%.c
	$(CC) $(CFLAGS) $(INC_DIRS) -shared -o $@ $<

# Build Shared librarys in sched/src/ #
$(SCHED_SRC_DIR)/lib%.so: $(SCHED_SRC_DIR)/%.c
	$(CC) $(CFLAGS) $(INC_DIRS) -shared -o $@ $<

# Build Test object file #
$(TEST_DIR)/sched_test.o: $(TEST_DIR)/sched_test.c
	$(CC) $(CFLAGS) $(INC_DIRS) -c -o $@ $<

# build directory before linking (order-only prerequisite) #
$(TARGET): | $(BUILD_DIR)

# Test executable linked with shared library #
$(TARGET): $(OBJS) $(DS_SO_FILES) $(SCHED_SO_FILE)
	$(CC) -o $@ $^ -L$(DS_SRC_DIR) -L$(SCHED_SRC_DIR) $(addprefix -l, $(LIB_NAMES)) $(addprefix -l, $(SCHED_NAMES)) -Wl,-rpath,$(abspath $(DS_SRC_DIR)):$(abspath $(SCHED_SRC_DIR))

-include $(DS_SO_FILES:.so=.d) $(SCHED_SO_FILE:.so=.d) $(OBJS:.o=.d)





