# make/calculator.mak

# -------- Profile & flags (short commands: make debug / make release) --------
PROFILE ?= debug

ifeq ($(PROFILE),debug)
  CFLAGS ?= -g -O0 -std=c89 -Wall -Wextra -pedantic
else
  CFLAGS ?= -O2 -DNDEBUG -std=c89 -Wall -Wextra -pedantic
endif

BUILD_DIR := bin/$(PROFILE)

# -------- Project layout --------
DS_INC_DIR    = ../ds/inc
DS_SRC_DIR    = ../ds/src
CALC_SRC_DIR  = calculator/src
CALC_INC_DIR  = calculator/inc
TEST_DIR      = calculator/test

INC_DIRS = -I$(CALC_INC_DIR) -I$(DS_INC_DIR)

DS_LIB_NAMES   = stack
CALC_LIB_NAMES = calculator

DS_SO_FILES   = $(addprefix $(DS_SRC_DIR)/lib,$(addsuffix .so,$(DS_LIB_NAMES)))
CALC_SO_FILES = $(addprefix $(CALC_SRC_DIR)/lib,$(addsuffix .so,$(CALC_LIB_NAMES)))

TARGET = $(BUILD_DIR)/calculator_test.out
OBJS   = $(TEST_DIR)/calculator_test.o

# -------- Default goal --------
.DEFAULT_GOAL := all

# -------- Rules --------
$(BUILD_DIR):
	mkdir -p $@

# Build DS shared libs (../ds/src/libstack.so)
$(DS_SRC_DIR)/lib%.so: $(DS_SRC_DIR)/%.c
	$(CC) $(CFLAGS) $(INC_DIRS) -fPIC -shared -o $@ $<

# Build calc shared lib (calculator/src/libcalculator.so), link vs libstack + embed rpath to ds/src
$(CALC_SRC_DIR)/lib%.so: $(CALC_SRC_DIR)/%.c $(DS_SO_FILES)
	$(CC) $(CFLAGS) $(INC_DIRS) -fPIC -shared -o $@ $< \
	    -L$(DS_SRC_DIR) -lstack \
	    -Wl,-rpath,'$$ORIGIN/../../../ds/src'

# Build test object
$(TEST_DIR)/%.o: $(TEST_DIR)/%.c
	$(CC) $(CFLAGS) $(INC_DIRS) -c -o $@ $<

# Ensure build dir exists before linking
$(TARGET): | $(BUILD_DIR)

# Link test exe: put -lcalculator BEFORE -lstack; embed rpath to both lib dirs
$(TARGET): $(OBJS) $(DS_SO_FILES) $(CALC_SO_FILES)
	$(CC) -o $@ $(OBJS) \
	      -L$(CALC_SRC_DIR) -L$(DS_SRC_DIR) \
	      $(addprefix -l,$(CALC_LIB_NAMES)) \
	      $(addprefix -l,$(DS_LIB_NAMES)) \
	      -lm \
	      -Wl,-rpath,'$$ORIGIN/../../calculator/src:$$ORIGIN/../../../ds/src'

# -------- Convenience targets --------
.PHONY: all debug release run clean
all: $(TARGET)

debug:
	@$(MAKE) -f $(firstword $(MAKEFILE_LIST)) PROFILE=debug all

release:
	@$(MAKE) -f $(firstword $(MAKEFILE_LIST)) PROFILE=release all

run:
	@$(MAKE) -f $(firstword $(MAKEFILE_LIST)) PROFILE=$(PROFILE) all
	./$(TARGET)

clean:
	@rm -rf bin
	@rm -f $(DS_SO_FILES) $(CALC_SO_FILES) $(OBJS)

# Optional auto-deps (harmless if missing)
-include $(DS_SO_FILES:.so=.d) $(CALC_SO_FILES:.so=.d) $(OBJS:.o=.d)
