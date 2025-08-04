TARGET = $(word 1, $(MAKECMDGOALS))
ACTION = $(word 2, $(MAKECMDGOALS))

CC = gcc
CFLAGS_DEBUG := -ansi -pedantic-errors -Wall -Wextra -fPIC -g -MMD
CFLAGS_RELEASE := -ansi -pedantic-errors -Wall -Wextra -fPIC -DNDEBUG -O3 -MMD

BIN_DEBUG_DIR = bin/debug
BIN_RELEASE_DIR = bin/release

PROJECTS := $(notdir $(basename $(wildcard make/*)))

.PHONY: $(TARGET) clean

ifeq ($(ACTION),all)
$(TARGET):
	@mkdir -p $(BIN_RELEASE_DIR)
	@$(MAKE) -s -f make/$(TARGET).mak CFLAGS="$(CFLAGS_RELEASE)"
	@mkdir -p $(BIN_DEBUG_DIR)
	@$(MAKE) -s -f make/$(TARGET).mak CFLAGS="$(CFLAGS_DEBUG)"
else ifeq ($(ACTION),test)
$(TARGET):
	@mkdir -p $(BIN_DEBUG_DIR)
	@$(MAKE) -s -f make/$(TARGET).mak CFLAGS="$(CFLAGS_DEBUG)"
	./$(BIN_DEBUG_DIR)/$(TARGET)_test.out
else ifeq ($(ACTION),debug)
$(TARGET):
	@mkdir -p $(BIN_DEBUG_DIR)
	@$(MAKE) -s -f make/$(TARGET).mak CFLAGS="$(CFLAGS_DEBUG)"
else ifeq ($(ACTION),release)
$(TARGET):
	@mkdir -p $(BIN_RELEASE_DIR)
	@$(MAKE) -s -f make/$(TARGET).mak CFLAGS="$(CFLAGS_RELEASE)"
endif

clean:
	@rm -rf bin
	@for dir in $(PROJECTS); do \
		rm -f $$dir/src/*.o $$dir/src/*.so $$dir/src/*.d || true; \
		rm -f $$dir/test/*.o $$dir/test/*.d || true; \
	done
	rm -f *.out
	





