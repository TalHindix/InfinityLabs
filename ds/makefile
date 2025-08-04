TARGET = $(word 1, $(MAKECMDGOALS))
ACTION = $(word 2, $(MAKECMDGOALS))

CC = gcc
CFLAGS_DEBUG := -ansi -pedantic-errors -Wall -Wextra -fPIC -g -MMD
CFLAGS_RELEASE := -ansi -pedantic-errors -Wall -Wextra -fPIC -DNDEBUG -O3 -MMD

BIN_DEBUG_DIR = bin/debug
BIN_RELEASE_DIR = bin/release

.PHONY: clean debug release test all

ifeq ($(ACTION),all)
$(TARGET):
	@echo "Building $(TARGET) in RELEASE mode..."
	@mkdir -p $(BIN_RELEASE_DIR)
	@$(MAKE) -s -f make/$(TARGET).mak CFLAGS="$(CFLAGS_RELEASE)"
	@echo "Building $(TARGET) in DEBUG mode..."
	@mkdir -p $(BIN_DEBUG_DIR)
	@$(MAKE) -s -f make/$(TARGET).mak CFLAGS="$(CFLAGS_DEBUG)"
	@echo "Build complete."
else ifeq ($(ACTION),test)
$(TARGET):
	@echo "Building $(TARGET) in DEBUG mode and running tests..."
	@mkdir -p $(BIN_DEBUG_DIR)
	@$(MAKE) -s -f make/$(TARGET).mak CFLAGS="$(CFLAGS_DEBUG)"
	./$(BIN_DEBUG_DIR)/$(TARGET)_test.out
else ifeq ($(ACTION),debug)
$(TARGET):
	@echo "Building $(TARGET) in DEBUG mode..."
	@mkdir -p $(BIN_DEBUG_DIR)
	@$(MAKE) -s -f make/$(TARGET).mak CFLAGS="$(CFLAGS_DEBUG)"
else ifeq ($(ACTION),release)
$(TARGET):
	@echo "Building $(TARGET) in RELEASE mode..."
	@mkdir -p $(BIN_RELEASE_DIR)
	@$(MAKE) -s -f make/$(TARGET).mak CFLAGS="$(CFLAGS_RELEASE)"
endif

clean:
	@echo "Cleaning bin directory..."
	@rm -rf bin

