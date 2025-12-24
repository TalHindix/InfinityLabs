# ILRD Final Project | Tal Hindi | 08/12/2025

#── Compiler ───────────────────────────────────────────────────────────────────
CXX      := g++
CXXFLAGS := -std=c++17 -Wall -Wextra -Wpedantic -fPIC
LDFLAGS  := -pthread -ldl

#── Directories ────────────────────────────────────────────────────────────────
FW_SRC    := framework/src
FW_INC    := framework/inc
FW_TEST   := framework/test
CONC_SRC  := concrete/src
CONC_INC  := concrete/inc
CONC_TEST := concrete/test
PLUGINS   := plugins
OBJ       := obj
BIN       := bin
LIB       := lib

#── Sources & Objects ──────────────────────────────────────────────────────────
FW_SRCS     := $(wildcard $(FW_SRC)/*.cpp)
FW_OBJS     := $(FW_SRCS:$(FW_SRC)/%.cpp=$(OBJ)/fw/%.o)
CONC_SRCS   := $(wildcard $(CONC_SRC)/*.cpp)
CONC_OBJS   := $(CONC_SRCS:$(CONC_SRC)/%.cpp=$(OBJ)/conc/%.o)
FW_TESTS    := $(wildcard $(FW_TEST)/*.cpp)
FW_BINS     := $(FW_TESTS:$(FW_TEST)/%.cpp=$(BIN)/%)
CONC_TESTS  := $(wildcard $(CONC_TEST)/*.cpp)
CONC_BINS   := $(CONC_TESTS:$(CONC_TEST)/%.cpp=$(BIN)/%)
ALL_BINS    := $(FW_BINS) $(CONC_BINS)
PLUGIN_SRCS := $(wildcard $(PLUGINS)/*.cpp)
PLUGIN_LIBS := $(PLUGIN_SRCS:$(PLUGINS)/%.cpp=$(PLUGINS)/lib%.so)

#── Libraries ──────────────────────────────────────────────────────────────────
FW_LIB   := $(LIB)/libilrd_framework.so
CONC_LIB := $(LIB)/libilrd_concrete.so
INCLUDES := -I$(FW_INC) -I$(CONC_INC)
RPATH    := -Wl,-rpath,'$$ORIGIN/../$(LIB)'

#── Build Modes ────────────────────────────────────────────────────────────────
DEBUG   := -g -O0 -DDEBUG
RELEASE := -O2 -DNDEBUG
TSAN    := -fsanitize=thread
ASAN    := -fsanitize=address -fsanitize=undefined

#── Targets ────────────────────────────────────────────────────────────────────
.PHONY: all libs tests plugins clean debug release tsan asan run run-all valgrind

all: CXXFLAGS += $(DEBUG)
all: libs tests plugins

#── Libraries Build ────────────────────────────────────────────────────────────
libs: $(FW_LIB) $(CONC_LIB)

$(FW_LIB): $(FW_OBJS) | $(LIB)
	@echo "[SO] $@"
	@$(CXX) -shared $(LDFLAGS) $^ -o $@

$(CONC_LIB): $(CONC_OBJS) | $(FW_LIB)
ifneq ($(CONC_OBJS),)
	@echo "[SO] $@"
	@$(CXX) -shared $(LDFLAGS) $^ -L$(LIB) -lilrd_framework -o $@
else
	@touch $@
endif

#── Object Files ───────────────────────────────────────────────────────────────
$(OBJ)/fw/%.o: $(FW_SRC)/%.cpp | $(OBJ)/fw
	@echo "[CC] $<"
	@$(CXX) $(CXXFLAGS) -MMD -MP $(INCLUDES) -c $< -o $@

$(OBJ)/conc/%.o: $(CONC_SRC)/%.cpp | $(OBJ)/conc
	@echo "[CC] $<"
	@$(CXX) $(CXXFLAGS) -MMD -MP $(INCLUDES) -c $< -o $@

#── Tests ──────────────────────────────────────────────────────────────────────
tests: libs $(ALL_BINS)

$(BIN)/%: $(FW_TEST)/%.cpp | $(FW_LIB) $(BIN)
	@echo "[LD] $@"
	@$(CXX) $(CXXFLAGS) $(INCLUDES) $< -L$(LIB) -lilrd_framework $(RPATH) -ldl -o $@

$(BIN)/%: $(CONC_TEST)/%.cpp | $(CONC_LIB) $(BIN)
	@echo "[LD] $@"
	@$(CXX) $(CXXFLAGS) $(INCLUDES) $< -L$(LIB) -lilrd_concrete -lilrd_framework $(RPATH) -ldl -o $@

#── Plugins (hot-loadable with -D__HANDLETON__) ────────────────────────────────
plugins: $(PLUGIN_LIBS)

$(PLUGINS)/lib%.so: $(PLUGINS)/%.cpp | $(FW_LIB)
	@mkdir -p $(PLUGINS)
	@echo "[PLUGIN] $@"
	@$(CXX) $(CXXFLAGS) -D__HANDLETON__ -shared $(INCLUDES) $< -o $@

#── Directories ────────────────────────────────────────────────────────────────
$(OBJ)/fw $(OBJ)/conc $(BIN) $(LIB):
	@mkdir -p $@

#── Build Modes ────────────────────────────────────────────────────────────────
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

#── Run ────────────────────────────────────────────────────────────────────────
TEST ?= $(firstword $(notdir $(ALL_BINS)))

run: all
	@./$(BIN)/$(TEST)

run-all: all
	@for t in $(ALL_BINS); do echo ">>> $$t"; ./$$t || exit 1; done

valgrind: all
	@valgrind --leak-check=full --track-origins=yes ./$(BIN)/$(TEST)

#── Clean ──────────────────────────────────────────────────────────────────────
clean:
	@rm -rf $(OBJ) $(BIN) $(LIB) $(PLUGINS)/*.so log_file
	@echo "Clean"

#── Help ───────────────────────────────────────────────────────────────────────
help:
	@echo ""
	@echo "ILRD Final Project - Build System"
	@echo "=================================="
	@echo ""
	@echo "Usage: make [target] [options]"
	@echo ""
	@echo "Build Targets:"
	@echo "  all         Build everything: libs + tests + plugins (default, debug mode)"
	@echo "  libs        Build shared libraries only (framework + concrete)"
	@echo "  tests       Build test executables"
	@echo "  plugins     Build hot-loadable plugins with -D__HANDLETON__"
	@echo ""
	@echo "Build Modes:"
	@echo "  debug       Clean rebuild with debug flags (-g -O0)"
	@echo "  release     Clean rebuild with optimization (-O2)"
	@echo "  tsan        Clean rebuild with ThreadSanitizer"
	@echo "  asan        Clean rebuild with AddressSanitizer"
	@echo ""
	@echo "Run:"
	@echo "  run              Run first test (or specify TEST=name)"
	@echo "  run TEST=x       Run specific test:  make run TEST=framework_test"
	@echo "  run-all          Run all tests sequentially"
	@echo "  valgrind         Run test with valgrind memory check"
	@echo ""
	@echo "Utility:"
	@echo "  clean       Remove all build artifacts (obj/, bin/, lib/, plugins/*.so)"
	@echo "  help        Show this message"
	@echo ""
	@echo "Project Structure:"
	@echo "  framework/      Generic reusable components (Reactor, ThreadPool, etc.)"
	@echo "  concrete/       Project-specific implementations (NBD, RAID, etc.)"
	@echo "  plugins/        Hot-loadable plugins (monitored by DirMonitor)"
	@echo ""
	@echo "Plugin Workflow:"
	@echo "  1. Create plugin:    plugins/my_plugin.cpp"
	@echo "  2. Build:            make plugins"
	@echo "  3. Hot-load test:    cp plugins/libmy_plugin.so plugins/libcopy.so"
	@echo "                       (DirMonitor detects new .so files at runtime)"
	@echo ""

-include $(OBJ)/fw/*.d $(OBJ)/conc/*.d