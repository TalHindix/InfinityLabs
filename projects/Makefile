# Directories *******************************************************************

SRC_DIR := src
TEST_DIR := test
INCLUDE_DIR := inc

DS_INC_DIR := ../ds/inc
DS_SRC_FILES := \
    ../ds/src/pq.c \
    ../ds/src/task.c \
    ../ds/src/uid.c \
    ../ds/src/sortedl.c \
    ../ds/src/dll.c

# Compiler Setup ****************************************************************

CC := gcc
DEBUG := -ansi -pedantic-errors -Wall -Wextra -g
RELEASE := -ansi -pedantic-errors -Wall -Wextra -DNDEBUG -O3
CFLAGS := $(DEBUG)
INCLUDES := -I$(INCLUDE_DIR) -I$(DS_INC_DIR)

# Source Files ******************************************************************

SRC_FILES := $(wildcard $(SRC_DIR)/*.c)
TEST_FILES := $(wildcard $(TEST_DIR)/*_test.c)

PROJECTS := $(notdir $(basename $(TEST_FILES)))
EXECUTABLES := $(PROJECTS:%=%)

SRC_OBJS := $(SRC_FILES:.c=.o)
TEST_OBJS := $(TEST_FILES:.c=.o)
OBJS := $(SRC_OBJS) $(TEST_OBJS)

# Targets ***********************************************************************

all: $(EXECUTABLES)

%.out: $(SRC_DIR)/%.c $(TEST_DIR)/%_test.c
	$(CC) $(CFLAGS) $(INCLUDES) -o $@ $^ $(DS_SRC_FILES)

objects: $(OBJS)

%.o: %.c
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

# Phony Targets *****************************************************************

debug: CFLAGS := $(DEBUG)
debug: clean all

release: CFLAGS := $(RELEASE)
release: clean all

clean:
	rm -f $(SRC_DIR)/*.o $(TEST_DIR)/*.o *.out

.PHONY: all clean debug release objects \
        $(PROJECTS) \
        $(PROJECTS:%=release-%) $(PROJECTS:%=debug-%)

