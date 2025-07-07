# Variables ********************************************************************

SRC_DIR := src
TEST_DIR := test
INCLUDE_DIR := inc

CC := gcc
DEBUG := -ansi -pedantic-errors -Wall -Wextra -g
RELEASE := -ansi -pedantic-errors -Wall -Wextra -DNDEBUG -O3
CFLAGS := $(DEBUG)
INCLUDES := -I$(INCLUDE_DIR)

SRC_FILES := $(wildcard $(SRC_DIR)/*.c)
TEST_FILES := $(wildcard $(TEST_DIR)/*_test.c)

# notedir - remove directories from filename, basename remove ending from filename (e.g dir/file.c -> file)
PROJECTS := $(notdir $(basename $(TEST_FILES)))
EXECUTABLES := $(PROJECTS:%=%)

SRC_OBJS := $(SRC_FILES:.c=.o)
TEST_OBJS := $(TEST_FILES:.c=.o)
OBJS := $(SRC_OBJS) $(TEST_OBJS)

# Targets ***********************************************************************

all: $(EXECUTABLES)

%.out: $(SRC_DIR)/%.c $(TEST_DIR)/%_test.c
	$(CC) $(CFLAGS) $(INCLUDES) -o $@ $^

queue.out: $(SRC_DIR)/queue.c $(SRC_DIR)/sll.c $(TEST_DIR)/queue_test.c
	$(CC) $(CFLAGS) $(INCLUDES) -o $@ $^

objects: $(OBJS)

%.o: %.c
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

# .PONEY *************************************************************************

debug: CFLAGS := $(DEBUG)
debug: clean all

release: CFLAGS := $(RELEASE)
release: clean all

clean:
	rm -f $(SRC_DIR)/*.o $(TEST_DIR)/*.o *.out

.PHONY: all clean debug release objects \
        $(PROJECTS) \
        $(PROJECTS:%=release-%) $(PROJECTS:%=debug-%)

