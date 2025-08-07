CC = gcc

SRC_DIR = src
INC_DIR = inc
DEBUG_DIR = bin/debug
RELEASE_DIR = bin/release
TEST_DIR = test

DEBUG_CFLAGS = -ansi -pedantic-errors -Wall -Wextra -g -O0 -fPIC -I$(INC_DIR)
RELEASE_CFLAGS = -ansi -pedantic-errors -Wall -Wextra -DNDEBUG -O3 -fPIC -I$(INC_DIR)
LDFLAGS = -shared

# Project source files (excluding _test.c)
PROJECT_SRCS := $(wildcard $(SRC_DIR)/*.c)
PROJECTS := $(basename $(notdir $(PROJECT_SRCS)))

# Decide on compilation mod, default: debug.
MODE ?= debug

ifeq ($(MODE), debug)
	CFLAGS = $(DEBUG_CFLAGS)
	BIN_DIR = $(DEBUG_DIR)
else ifeq ($(MODE), release)
	CFLAGS = $(RELEASE_CFLAGS)
	BIN_DIR = $(RELEASE_DIR)
endif

# Rules for creation of all debug and release so and tests.
all: debug

debug:
	$(MAKE) MODE=debug build-all

release:
	$(MAKE) MODE=release build-all

build-all: $(addprefix $(BIN_DIR)/lib, $(addsuffix .so, $(PROJECTS))) test

# Generate so rules and their dependencies
define GEN_SO_RULE
DEPS_$(1) := $(shell \
	headers=`$(CC) -MM $(SRC_DIR)/$(1).c -I$(INC_DIR) | sed 's/.*://g'`; \
	for h in $$headers; do \
		echo $$h; \
		base=$$(basename $$h .h); \
		if [ -f $(SRC_DIR)/$$base.c ]; then echo $(SRC_DIR)/$$base.c; fi; \
	done \
)

$(BIN_DIR)/lib$(1).so: $$(DEPS_$(1))
	@echo "🔧 Linking $$@"
	$$(CC) $$(CFLAGS) $$(LDFLAGS) -o $$@ $$^
endef

$(foreach mod,$(PROJECTS),$(eval $(call GEN_SO_RULE,$(mod))))

# Test files
TEST_SRCS := $(wildcard $(TEST_DIR)/*_test.c)
TESTS := $(basename $(notdir $(TEST_SRCS)))
TEST_BINS := $(addprefix $(BIN_DIR)/, $(TESTS))

# Build all test executables
test: $(TEST_BINS)

# Rule for building a single test binary
$(BIN_DIR)/%_test: $(TEST_DIR)/%_test.c
	@echo "🧪 Building test: $@"
	$(CC) $(CFLAGS) -o $@ $< -L$(BIN_DIR) -Wl,-rpath=$(BIN_DIR) -l$(basename $*)

# Run all tests (works on debug only)
run: test
	@for t in $(TEST_BINS); do \
		echo "▶ Running $$t"; \
		LD_LIBRARY_PATH=$(BIN_DIR) $$t || echo "❌ $$t failed"; \
	done

# Run a single test in debug mode for specific project
run-%: $(BIN_DIR)/%_test
	@echo "▶ Running test: $*_test"
	@LD_LIBRARY_PATH=$(BIN_DIR) $(BIN_DIR)/$*_test

# Create release so and test for specific project
release-%:
	$(MAKE) MODE=release $*	

# Run a single test in release mode for specific project
run-release-%:
	@echo "▶ Running test: $*_test"
	@LD_LIBRARY_PATH=$(RELEASE_DIR) $(RELEASE_DIR)/$*_test

# Build shared lib and test for a specific project
$(PROJECTS):
	@$(MAKE) BIN_DIR=$(BIN_DIR) CFLAGS="$(CFLAGS)" $(BIN_DIR)/lib$@.so
	@if [ -f $(TEST_DIR)/$@_test.c ]; then \
		$(MAKE) BIN_DIR=$(BIN_DIR) CFLAGS="$(CFLAGS)" $(BIN_DIR)/$@_test; \
	fi

# Clean all binaries
clean:
	rm -rf $(DEBUG_DIR)/* $(RELEASE_DIR)/*

.PHONY: all build-all debug release test run clean $(PROJECTS)

	

