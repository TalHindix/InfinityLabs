CC = gcc

NAME = task
PROJECT_NAME = sched

DEBUG_CFLAGS = -ansi -pedantic-errors -Wall -Wextra -g -I../$(PROJECT_NAME)/inc -I../../ds/inc
RELEASE_CFLAGS = -ansi -pedantic-errors -Wall -Wextra -DNDEBUG -O3 -I../$(PROJECT_NAME)/inc -I../../ds/inc
CFLAGS = $(DEBUG_CFLAGS)

TARGET = a.out

SRC_OF_DEPENDENCY = ../../ds/src
SRC_DIR = ../$(PROJECT_NAME)/src
TEST_DIR = ../$(PROJECT_NAME)/test

SRC = $(SRC_OF_DEPENDENCY)/uid.c $(SRC_DIR)/$(NAME).c $(TEST_DIR)/$(NAME)_test.c
OBJS = $(SRC:.c=.o)

.PHONY: all clean debug release

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

debug:
	$(MAKE) -f $(NAME).mak CFLAGS='$(DEBUG_CFLAGS)' all

release:
	$(MAKE) -f $(NAME).mak CFLAGS='$(RELEASE_CFLAGS)' all

clean:
	rm -f $(OBJS) $(TARGET)

