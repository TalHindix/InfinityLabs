#******************************************************************************
# ILRD Final Project
# Developer: Tal Hindi | Date: 08/12/2025
#******************************************************************************

# Compiler
CXX      := g++
CXXFLAGS := -std=c++17 -Wall -Wextra -Wpedantic -fPIC
DEPFLAGS := -MMD -MP
LDFLAGS  := -pthread

# Directories
FW_SRC   := framework/src
FW_INC   := framework/inc
FW_TEST  := framework/test
CONC_SRC := concrete/src
CONC_INC := concrete/inc
CONC_TEST:= concrete/test
OBJ      := obj
BIN      := bin
LIB      := lib

# Sources & Objects
FW_SRCS  := $(wildcard $(FW_SRC)/*.cpp)
FW_OBJS  := $(FW_SRCS:$(FW_SRC)/%.cpp=$(OBJ)/fw/%.o)
CONC_SRCS:= $(wildcard $(CONC_SRC)/*.cpp)
CONC_OBJS:= $(CONC_SRCS:$(CONC_SRC)/%.cpp=$(OBJ)/conc/%.o)

# Tests
FW_TESTS := $(wildcard $(FW_TEST)/*.cpp)
FW_BINS  := $(FW_TESTS:$(FW_TEST)/%.cpp=$(BIN)/%)
CONC_TESTS := $(wildcard $(CONC_TEST)/*.cpp)
CONC_BINS  := $(CONC_TESTS:$(CONC_TEST)/%.cpp=$(BIN)/%)
ALL_BINS := $(FW_BINS) $(CONC_BINS)

# Libraries
FW_LIB   := $(LIB)/libilrd_framework.so
CONC_LIB := $(LIB)/libilrd_concrete.so
INCLUDES := -I$(FW_INC) -I$(CONC_INC)
RPATH    := -Wl,-rpath,'$$ORIGIN/../$(LIB)'

# Build Flags
DEBUG    := -g -O0 -DDEBUG
RELEASE  := -O2 -DNDEBUG
TSAN     := -fsanitize=thread
ASAN     := -fsanitize=address -fsanitize=undefined

#=============================================================================
.PHONY: all libs tests clean debug release tsan asan run run-all valgrind help

all: CXXFLAGS += $(DEBUG)
all: libs tests
	@echo "\n✓ Build complete: $(LIB)/ $(BIN)/\n"

#=============================================================================
# Libraries
#=============================================================================
libs: $(FW_LIB) $(CONC_LIB)

$(FW_LIB): $(FW_OBJS) | $(LIB)
	@echo "[SO] $@"
	@$(CXX) -shared $(LDFLAGS) $^ -o $@

$(CONC_LIB): $(CONC_OBJS) | $(FW_LIB)
	@echo "[SO] $@"
ifneq ($(CONC_OBJS),)
	@$(CXX) -shared $(LDFLAGS) $^ -L$(LIB) -lilrd_framework -o $@
else
	@touch $@
endif

#=============================================================================
# Objects (with dependency generation)
#=============================================================================
$(OBJ)/fw/%.o: $(FW_SRC)/%.cpp | $(OBJ)/fw
	@echo "[CC] $<"
	@$(CXX) $(CXXFLAGS) $(DEPFLAGS) $(INCLUDES) -c $< -o $@

$(OBJ)/conc/%.o: $(CONC_SRC)/%.cpp | $(OBJ)/conc
	@echo "[CC] $<"
	@$(CXX) $(CXXFLAGS) $(DEPFLAGS) $(INCLUDES) -c $< -o $@

#=============================================================================
# Tests
#=============================================================================
tests: libs $(ALL_BINS)

$(BIN)/%: $(FW_TEST)/%.cpp | $(FW_LIB) $(BIN)
	@echo "[LD] $@"
	@$(CXX) $(CXXFLAGS) $(INCLUDES) $< -L$(LIB) -lilrd_framework $(RPATH) -o $@

$(BIN)/%: $(CONC_TEST)/%.cpp | $(CONC_LIB) $(BIN)
	@echo "[LD] $@"
	@$(CXX) $(CXXFLAGS) $(INCLUDES) $< -L$(LIB) -lilrd_concrete -lilrd_framework $(RPATH) -o $@

#=============================================================================
# Directories
#=============================================================================
$(OBJ)/fw $(OBJ)/conc $(BIN) $(LIB):
	@mkdir -p $@

#=============================================================================
# Build Modes
#=============================================================================
debug: CXXFLAGS += $(DEBUG)
debug: clean all

release: CXXFLAGS += $(RELEASE)
release: clean all

tsan: CXXFLAGS += $(DEBUG) $(TSAN)
tsan: LDFLAGS += $(TSAN)
tsan: clean all

asan: CXXFLAGS += $(DEBUG) $(ASAN)
asan: LDFLAGS += $(ASAN)
asan: clean all

#=============================================================================
# Run
#=============================================================================
TEST ?= $(firstword $(notdir $(ALL_BINS)))

run: all
	@echo "\n>>> $(BIN)/$(TEST)"
	@./$(BIN)/$(TEST)

run-all: all
	@for t in $(ALL_BINS); do echo "\n>>> $$t"; ./$$t || exit 1; done
	@echo "\n✓ All tests passed\n"

valgrind: all
	@valgrind --leak-check=full --track-origins=yes ./$(BIN)/$(TEST)

#=============================================================================
# Utility
#=============================================================================
clean:
	@rm -rf $(OBJ) $(BIN) $(LIB) log_file
	@echo "✓ Clean"

help:
	@echo "make [target]"
	@echo ""
	@echo "  all      - Build libs + tests (debug)"
	@echo "  libs     - Build .so only"
	@echo "  tests    - Build test executables"
	@echo "  clean    - Remove build artifacts"
	@echo ""
	@echo "  debug    - Debug build"
	@echo "  release  - Optimized build"
	@echo "  tsan     - ThreadSanitizer"
	@echo "  asan     - AddressSanitizer"
	@echo ""
	@echo "  run TEST=x    - Run specific test"
	@echo "  run-all       - Run all tests"
	@echo "  valgrind TEST=x"

# Auto-dependencies
-include $(OBJ)/fw/*.d $(OBJ)/conc/*.d