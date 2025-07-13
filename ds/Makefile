# ********************************************************************
# Makefile – Data-Structures Project
# ********************************************************************

# ---------- Paths ---------------------------------------------------
SRC_DIR     := src
TEST_DIR    := test
INCLUDE_DIR := inc

# ---------- Compiler & Flags ----------------------------------------
CC      := gcc
DEBUG   := -ansi -pedantic-errors -Wall -Wextra -g
RELEASE := -ansi -pedantic-errors -Wall -Wextra -DNDEBUG -O3

# --------------------------------------------------------------------
# Build type selection:
#   make … BUILD=release   → CFLAGS = $(RELEASE)
#   make …                 → CFLAGS = $(DEBUG)  (default)
# --------------------------------------------------------------------
BUILD ?= debug

ifeq ($(BUILD),release)
    CFLAGS := $(RELEASE)
else
    CFLAGS := $(DEBUG)
endif

INCLUDES := -I$(INCLUDE_DIR)

# ---------- Source & Test files -------------------------------------
SRC_FILES  := $(wildcard $(SRC_DIR)/*.c)
TEST_FILES := $(wildcard $(TEST_DIR)/*_test.c)

# Project names (e.g. queue_test.c → queue) --------------------------
PROJECTS     := $(notdir $(basename $(TEST_FILES)))
EXECUTABLES  := $(PROJECTS:%=%)

# Object files -------------------------------------------------------
SRC_OBJS  := $(SRC_FILES:.c=.o)
TEST_OBJS := $(TEST_FILES:.c=.o)
OBJS      := $(SRC_OBJS) $(TEST_OBJS)

# ********************************************************************
# Targets
# ********************************************************************

# Build all unit-test executables
all: $(EXECUTABLES)

# Pattern rule – build <name>.out from matching sources
%.out: $(SRC_DIR)/%.c $(TEST_DIR)/%_test.c
	$(CC) $(CFLAGS) $(INCLUDES) -o $@ $^

# Special rule for queue.out (uses extra source file)
queue.out: $(SRC_DIR)/queue.c $(SRC_DIR)/sll.c $(TEST_DIR)/queue_test.c
	$(CC) $(CFLAGS) $(INCLUDES) -o $@ $^
	
sortedl.out: $(SRC_DIR)/sortedl.c $(SRC_DIR)/dll.c $(TEST_DIR)/sortedl_test.c
	$(CC) $(CFLAGS) $(INCLUDES) -o $@ $^

# Build only object files (rarely needed)
objects: $(OBJS)

%.o: %.c
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

# --------------------------------------------------------------------
# Convenience targets
# --------------------------------------------------------------------
debug:   BUILD := debug
debug:   clean all            # make debug

release: BUILD := release
release: clean all            # make release

# Clean build artefacts
clean:
	@echo "Cleaning…"
	@rm -f $(SRC_DIR)/*.o $(TEST_DIR)/*.o *.out

# --------------------------------------------------------------------
.PHONY: all clean debug release objects $(PROJECTS)
# ********************************************************************
