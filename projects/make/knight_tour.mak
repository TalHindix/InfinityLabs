# make/knight_tour.mak

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
KNIGHT_TOUR_SRC_DIR  = knight_tour/src
KNIGHT_TOUR_INC_DIR  = knight_tour/inc
TEST_DIR             = knight_tour/test

INC_DIRS = -I$(KNIGHT_TOUR_INC_DIR) -I$(DS_INC_DIR)

DS_LIB_NAMES   = bit_arr
KNIGHT_TOUR_LIB_NAMES = knight_tour

DS_SO_FILES   = $(addprefix $(DS_SRC_DIR)/lib,$(addsuffix .so,$(DS_LIB_NAMES)))
KNIGHT_TOUR_SO_FILES = $(addprefix $(KNIGHT_TOUR_SRC_DIR)/lib,$(addsuffix .so,$(KNIGHT_TOUR_LIB_NAMES)))

TARGET = $(BUILD_DIR)/knight_tour_test.out
OBJS   = $(TEST_DIR)/knight_tour_test.o

# -------- Default goal --------
.DEFAULT_GOAL := all

# -------- Rules --------
$(BUILD_DIR):
	mkdir -p $@

# Build DS shared libs (../ds/src/libbit_arr.so)
$(DS_SRC_DIR)/lib%.so: $(DS_SRC_DIR)/%.c
	$(CC) $(CFLAGS) $(INC_DIRS) -fPIC -shared -o $@ $<

# Build knight_tour shared lib, link vs libbit_arr + embed rpath to ds/src
$(KNIGHT_TOUR_SRC_DIR)/lib%.so: $(KNIGHT_TOUR_SRC_DIR)/%.c $(DS_SO_FILES)
	$(CC) $(CFLAGS) $(INC_DIRS) -fPIC -shared -o $@ $< \
	    -L$(DS_SRC_DIR) -lbit_arr \
	    -Wl,-rpath,'$$ORIGIN/../../../ds/src'

# Build test object
$(TEST_DIR)/%.o: $(TEST_DIR)/%.c
	$(CC) $(CFLAGS) $(INC_DIRS) -c -o $@ $<

# Ensure build dir exists before linking
$(TARGET): | $(BUILD_DIR)

# Link test exe: put -lknight_tour BEFORE -lbit_arr; embed rpath to both lib dirs
$(TARGET): $(OBJS) $(DS_SO_FILES) $(KNIGHT_TOUR_SO_FILES)
	$(CC) -o $@ $(OBJS) \
	      -L$(KNIGHT_TOUR_SRC_DIR) -L$(DS_SRC_DIR) \
	      $(addprefix -l,$(KNIGHT_TOUR_LIB_NAMES)) \
	      $(addprefix -l,$(DS_LIB_NAMES)) \
	      -lm \
	      -Wl,-rpath,'$$ORIGIN/../../knight_tour/src:$$ORIGIN/../../../ds/src'

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
	@rm -f $(DS_SO_FILES) $(KNIGHT_TOUR_SO_FILES) $(OBJS)

# Optional auto-deps (harmless if missing)
-include $(KNIGHT_TOUR_SO_FILES:.so=.d) $(OBJS:.o=.d)