# make/recursion.mak


PROFILE ?= debug

ifeq ($(PROFILE),debug)
  CFLAGS ?= -g -O0 -std=c89 -Wall -Wextra -pedantic
else
  CFLAGS ?= -O2 -DNDEBUG -std=c89 -Wall -Wextra -pedantic
endif

BUILD_DIR := bin/$(PROFILE)

# -------- Project layout --------
DHCP_SRC_DIR  = dhcp/src
DHCP_INC_DIR  = dhcp/inc
TEST_DIR      = dhcp/test

INC_DIRS = -I$(DHCP_INC_DIR)


DHCP_LIB_NAMES = dhcp trie

DHCP_SO_FILES = $(addprefix $(DHCP_SRC_DIR)/lib,$(addsuffix .so,$(DHCP_LIB_NAMES)))

TARGET = $(BUILD_DIR)/dhcp_test.out
OBJS   = $(TEST_DIR)/dhcp_test.o

# -------- Default goal --------
.DEFAULT_GOAL := all

# -------- Rules --------
$(BUILD_DIR):
	mkdir -p $@


# Build DHCP shared lib
$(DHCP_SRC_DIR)/lib%.so: $(DHCP_SRC_DIR)/%.c
	$(CC) $(CFLAGS) $(INC_DIRS) -fPIC -shared -o $@ $<

# Build test object
$(TEST_DIR)/%.o: $(TEST_DIR)/%.c
	$(CC) $(CFLAGS) $(INC_DIRS) -c -o $@ $<

# Ensure build dir exists before linking
$(TARGET): | $(BUILD_DIR)

# Link test exe
$(TARGET): $(OBJS) $(DHCP_SO_FILES)
	$(CC) -o $@ $(OBJS) \
	      -L$(DHCP_SRC_DIR) \
	      $(addprefix -l,$(DHCP_LIB_NAMES)) \
	      -lm \
	      -Wl,-rpath,'$$ORIGIN/../../dhcp/src'

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
	@rm -f $(DHCP_SO_FILES) $(OBJS)

# Optional auto-deps (harmless if missing)
-include $(DHCP_SO_FILES:.so=.d) $(OBJS:.o=.d)
