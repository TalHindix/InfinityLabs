# --- Compiler and Flags ---
CC = gcc
CFLAGS_DEBUG   = -ansi -pedantic-errors -Wall -Wextra -g
CFLAGS_RELEASE = -ansi -pedantic-errors -Wall -Wextra -DNDEBUG -O3
LDFLAGS = -lpthread

# --- Build Mode (default: debug) ---
BUILD ?= debug
ifeq ($(BUILD),debug)
  CFLAGS = $(CFLAGS_DEBUG)
else ifeq ($(BUILD),release)
  CFLAGS = $(CFLAGS_RELEASE)
endif

# --- Directories (relative to utils/make) ---
SRC_DIR   = src
INC_DIR   = inc
TEST_DIR  = test
BIN_DIR   = bin/$(BUILD)
DS_DIR    = ../ds/src
SCHED_DIR = ../projects/sched/src

# --- Includes ---
CFLAGS += -I$(INC_DIR) -I../projects/sched/inc -I../ds/inc

# --- Objects ---
WD_OBJS    = $(SRC_DIR)/wd_client.o $(SRC_DIR)/wd.o $(SRC_DIR)/wd_lib.o
SCHED_OBJS = $(SCHED_DIR)/task.o $(SCHED_DIR)/sched.o
DS_OBJS    = $(DS_DIR)/vector.o $(DS_DIR)/heap.o $(DS_DIR)/pq.o $(DS_DIR)/uid.o

LIBWD = libwd.a

# --- Targets ---
all: $(BIN_DIR)/wd $(BIN_DIR)/wd_test

# Watchdog process executable
$(BIN_DIR)/wd: $(SRC_DIR)/wd.o $(LIBWD) $(SCHED_OBJS) $(DS_OBJS) | $(BIN_DIR)
	$(CC) $(CFLAGS) -o $@ $(SRC_DIR)/wd.o $(LIBWD) $(SCHED_OBJS) $(DS_OBJS) $(LDFLAGS)

# Test executable
$(BIN_DIR)/wd_test: $(TEST_DIR)/wd_test.o $(LIBWD) $(SCHED_OBJS) $(DS_OBJS) | $(BIN_DIR)
	$(CC) $(CFLAGS) -o $@ $(TEST_DIR)/wd_test.o $(LIBWD) $(SCHED_OBJS) $(DS_OBJS) $(LDFLAGS)

# Static library
$(LIBWD): $(WD_OBJS)
	ar rcs $@ $^

# --- Compilation rules ---
$(SRC_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

$(TEST_DIR)/%.o: $(TEST_DIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

$(SCHED_DIR)/%.o: $(SCHED_DIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

$(DS_DIR)/%.o: $(DS_DIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

# --- Ensure bin directory exists ---
$(BIN_DIR):
	mkdir -p $(BIN_DIR)

# --- Clean ---
clean:
	rm -f $(SRC_DIR)/*.o $(TEST_DIR)/*.o $(SCHED_DIR)/*.o $(DS_DIR)/*.o $(LIBWD) \
	      ../bin/debug/* ../bin/release/*
