#******************************************************************************
# Project:     Final Project - Generic Build System
# Date:        08/12/2025
# Developer:   Tal Hindi
# Description: Generic Makefile supporting framework/concrete hierarchy,
#              shared libraries (.so), and modular component addition
#******************************************************************************

#==============================================================================
# Compiler Settings
#==============================================================================
CXX := g++
CXXFLAGS := -std=c++17 -Wall -Wextra -Wpedantic -fPIC -D__HANDLETON__
LDFLAGS := -pthread
DEPFLAGS = -MMD -MP

#==============================================================================
# Directory Structure
#==============================================================================
# Root directories
FW_DIR := framework
CONC_DIR := concrete

# Subdirectories
INC_DIR := inc
SRC_DIR := src
TEST_DIR := test

# Build directories
OBJ_DIR := obj
BIN_DIR := bin
LIB_DIR := lib

# Framework paths
FW_INC := $(FW_DIR)/$(INC_DIR)
FW_SRC := $(FW_DIR)/$(SRC_DIR)
FW_TEST := $(FW_DIR)/$(TEST_DIR)

# Concrete paths
CONC_INC := $(CONC_DIR)/$(INC_DIR)
CONC_SRC := $(CONC_DIR)/$(SRC_DIR)
CONC_TEST := $(CONC_DIR)/$(TEST_DIR)

# Object subdirectories
FW_OBJ := $(OBJ_DIR)/framework
CONC_OBJ := $(OBJ_DIR)/concrete
TEST_OBJ := $(OBJ_DIR)/test

#==============================================================================
# Include Paths
#==============================================================================
INCLUDES := -I$(FW_INC) -I$(CONC_INC)

#==============================================================================
# Source Files Discovery
#==============================================================================
# Framework sources (excluding tests)
FW_SRCS := $(wildcard $(FW_SRC)/*.cpp)
FW_OBJS := $(patsubst $(FW_SRC)/%.cpp,$(FW_OBJ)/%.o,$(FW_SRCS))

# Concrete sources (excluding tests)
CONC_SRCS := $(wildcard $(CONC_SRC)/*.cpp)
CONC_OBJS := $(patsubst $(CONC_SRC)/%.cpp,$(CONC_OBJ)/%.o,$(CONC_SRCS))

# Test sources
FW_TEST_SRCS := $(wildcard $(FW_TEST)/*.cpp)
CONC_TEST_SRCS := $(wildcard $(CONC_TEST)/*.cpp)
ALL_TEST_SRCS := $(FW_TEST_SRCS) $(CONC_TEST_SRCS)

# Test objects
FW_TEST_OBJS := $(patsubst $(FW_TEST)/%.cpp,$(TEST_OBJ)/fw_%.o,$(FW_TEST_SRCS))
CONC_TEST_OBJS := $(patsubst $(CONC_TEST)/%.cpp,$(TEST_OBJ)/conc_%.o,$(CONC_TEST_SRCS))

# Test executables
FW_TEST_BINS := $(patsubst $(FW_TEST)/%.cpp,$(BIN_DIR)/%,$(FW_TEST_SRCS))
CONC_TEST_BINS := $(patsubst $(CONC_TEST)/%.cpp,$(BIN_DIR)/%,$(CONC_TEST_SRCS))
ALL_TEST_BINS := $(FW_TEST_BINS) $(CONC_TEST_BINS)

# Dependency files
DEPS := $(FW_OBJS:.o=.d) $(CONC_OBJS:.o=.d) $(FW_TEST_OBJS:.o=.d) $(CONC_TEST_OBJS:.o=.d)

#==============================================================================
# Shared Library Settings
#==============================================================================
FW_LIB_NAME := ilrd_framework
CONC_LIB_NAME := ilrd_concrete

FW_LIB := $(LIB_DIR)/lib$(FW_LIB_NAME).so
CONC_LIB := $(LIB_DIR)/lib$(CONC_LIB_NAME).so

# Runtime library path
RPATH := -Wl,-rpath,'$$ORIGIN/../$(LIB_DIR)'

#==============================================================================
# Build Configurations
#==============================================================================
# Debug build (default)
DEBUG_FLAGS := -g -O0 -DDEBUG

# Release build
RELEASE_FLAGS := -O2 -DNDEBUG

# Sanitizers
TSAN_FLAGS := -fsanitize=thread
ASAN_FLAGS := -fsanitize=address -fsanitize=undefined

#==============================================================================
# Phony Targets
#==============================================================================
.PHONY: all clean debug release tests libs run help
.PHONY: tsan asan valgrind
.PHONY: dirs fw_lib conc_lib
.PHONY: list-tests

#==============================================================================
# Default Target
#==============================================================================
all: CXXFLAGS += $(DEBUG_FLAGS)
all: dirs libs tests
	@echo ""
	@echo "========================================="
	@echo "  Build completed successfully!"
	@echo "========================================="
	@echo "  Libraries: $(LIB_DIR)/"
	@echo "  Tests:     $(BIN_DIR)/"
	@echo "========================================="

#==============================================================================
# Directory Creation
#==============================================================================
dirs:
	@mkdir -p $(FW_OBJ) $(CONC_OBJ) $(TEST_OBJ) $(BIN_DIR) $(LIB_DIR)

#==============================================================================
# Library Targets
#==============================================================================
libs: fw_lib conc_lib

fw_lib: dirs $(FW_LIB)

conc_lib: dirs fw_lib $(CONC_LIB)

# Framework shared library
$(FW_LIB): $(FW_OBJS)
	@echo "[LD] Creating framework library: $@"
	@if [ -n "$(FW_OBJS)" ] && [ "$(FW_OBJS)" != "" ]; then \
		$(CXX) -shared $(LDFLAGS) $^ -o $@; \
	else \
		echo "  (No framework sources - creating empty library placeholder)"; \
		touch $@; \
	fi

# Concrete shared library (depends on framework)
$(CONC_LIB): $(CONC_OBJS) | $(FW_LIB)
	@echo "[LD] Creating concrete library: $@"
	@if [ -n "$(CONC_OBJS)" ] && [ "$(CONC_OBJS)" != "" ]; then \
		$(CXX) -shared $(LDFLAGS) $^ -L$(LIB_DIR) -l$(FW_LIB_NAME) -o $@; \
	else \
		echo "  (No concrete sources - creating empty library placeholder)"; \
		touch $@; \
	fi

#==============================================================================
# Test Targets
#==============================================================================
tests: libs $(ALL_TEST_BINS)

# Framework test executables
$(BIN_DIR)/%: $(TEST_OBJ)/fw_%.o $(FW_LIB)
	@echo "[LD] Linking test: $@"
	$(CXX) $(LDFLAGS) $< -L$(LIB_DIR) -l$(FW_LIB_NAME) $(RPATH) -o $@

# Concrete test executables
$(BIN_DIR)/%: $(TEST_OBJ)/conc_%.o $(CONC_LIB) $(FW_LIB)
	@echo "[LD] Linking test: $@"
	$(CXX) $(LDFLAGS) $< -L$(LIB_DIR) -l$(CONC_LIB_NAME) -l$(FW_LIB_NAME) $(RPATH) -o $@

#==============================================================================
# Object File Compilation
#==============================================================================
# Framework objects
$(FW_OBJ)/%.o: $(FW_SRC)/%.cpp
	@echo "[CXX] Compiling: $<"
	$(CXX) $(CXXFLAGS) $(DEPFLAGS) $(INCLUDES) -c $< -o $@

# Concrete objects
$(CONC_OBJ)/%.o: $(CONC_SRC)/%.cpp
	@echo "[CXX] Compiling: $<"
	$(CXX) $(CXXFLAGS) $(DEPFLAGS) $(INCLUDES) -c $< -o $@

# Framework test objects
$(TEST_OBJ)/fw_%.o: $(FW_TEST)/%.cpp
	@echo "[CXX] Compiling test: $<"
	$(CXX) $(CXXFLAGS) $(DEPFLAGS) $(INCLUDES) -c $< -o $@

# Concrete test objects
$(TEST_OBJ)/conc_%.o: $(CONC_TEST)/%.cpp
	@echo "[CXX] Compiling test: $<"
	$(CXX) $(CXXFLAGS) $(DEPFLAGS) $(INCLUDES) -c $< -o $@

#==============================================================================
# Include Dependencies
#==============================================================================
-include $(DEPS)

#==============================================================================
# Plugins (for dlopen testing)
#==============================================================================
PLUGIN_DIR := plugins
PLUGIN_SRCS := $(wildcard $(PLUGIN_DIR)/*.cpp)
PLUGIN_LIBS := $(patsubst $(PLUGIN_DIR)/%.cpp,$(LIB_DIR)/lib%.so,$(PLUGIN_SRCS))

plugins: dirs $(FW_LIB) $(PLUGIN_LIBS)

$(LIB_DIR)/lib%.so: $(PLUGIN_DIR)/%.cpp $(FW_LIB)
	@echo "[LD] Building plugin: $@"
	$(CXX) $(CXXFLAGS) -shared $< -L$(LIB_DIR) -l$(FW_LIB_NAME) $(INCLUDES) -o $@

#==============================================================================
# Build Configurations
#==============================================================================
debug: CXXFLAGS += $(DEBUG_FLAGS)
debug: clean all

release: CXXFLAGS += $(RELEASE_FLAGS)
release: clean all

#==============================================================================
# Sanitizer Builds
#==============================================================================
tsan: CXXFLAGS += $(DEBUG_FLAGS) $(TSAN_FLAGS)
tsan: LDFLAGS += $(TSAN_FLAGS)
tsan: clean all

asan: CXXFLAGS += $(DEBUG_FLAGS) $(ASAN_FLAGS)
asan: LDFLAGS += $(ASAN_FLAGS)
asan: clean all

#==============================================================================
# Run Targets
#==============================================================================
# Run specific test: make run TEST=thread_pool_test
TEST ?= $(firstword $(notdir $(ALL_TEST_BINS)))

run: all
	@echo ""
	@echo "Running: $(BIN_DIR)/$(TEST)"
	@echo "========================================="
	@LD_LIBRARY_PATH=$(LIB_DIR):$$LD_LIBRARY_PATH ./$(BIN_DIR)/$(TEST)

# Run all tests
run-all: all
	@echo ""
	@echo "Running all tests..."
	@echo "========================================="
	@for test in $(ALL_TEST_BINS); do \
		echo ""; \
		echo ">>> Running: $$test"; \
		echo "-----------------------------------------"; \
		LD_LIBRARY_PATH=$(LIB_DIR):$$LD_LIBRARY_PATH ./$$test || exit 1; \
	done
	@echo ""
	@echo "========================================="
	@echo "  All tests passed!"
	@echo "========================================="

#==============================================================================
# Valgrind
#==============================================================================
valgrind: all
	@echo ""
	@echo "Running Valgrind on: $(BIN_DIR)/$(TEST)"
	@echo "========================================="
	@LD_LIBRARY_PATH=$(LIB_DIR):$$LD_LIBRARY_PATH \
		valgrind --leak-check=full --show-leak-kinds=all \
		--track-origins=yes ./$(BIN_DIR)/$(TEST)

#==============================================================================
# Utility Targets
#==============================================================================
clean:
	@echo "Cleaning build artifacts..."
	@rm -rf $(OBJ_DIR) $(BIN_DIR) $(LIB_DIR)
	@rm -f log_file
	@echo "Clean complete."

list-tests:
	@echo "Available tests:"
	@for test in $(notdir $(ALL_TEST_BINS)); do \
		echo "  - $$test"; \
	done

#==============================================================================
# Help
#==============================================================================
help:
	@echo "========================================"
	@echo "  Final Project Build System"
	@echo "========================================"
	@echo ""
	@echo "Usage: make [target] [options]"
	@echo ""
	@echo "Main Targets:"
	@echo "  all        - Build everything (debug mode)"
	@echo "  libs       - Build shared libraries only"
	@echo "  tests      - Build test executables"
	@echo "  clean      - Remove all build artifacts"
	@echo ""
	@echo "Build Configurations:"
	@echo "  debug      - Debug build with symbols"
	@echo "  release    - Optimized release build"
	@echo "  tsan       - Build with ThreadSanitizer"
	@echo "  asan       - Build with AddressSanitizer"
	@echo ""
	@echo "Run Targets:"
	@echo "  run        - Run single test (default: first found)"
	@echo "  run TEST=x - Run specific test"
	@echo "  run-all    - Run all tests"
	@echo "  valgrind   - Run with Valgrind memory checker"
	@echo ""
	@echo "Utility:"
	@echo "  list-tests - Show available test executables"
	@echo "  help       - Show this help message"
	@echo ""
	@echo "Examples:"
	@echo "  make                           # Build all"
	@echo "  make run TEST=logger_test      # Run specific test"
	@echo "  make tsan run TEST=logger_test"
	@echo "  make valgrind TEST=logger_test"
	@echo ""