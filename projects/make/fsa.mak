CC = gcc

NAME = fsa

DEBUG_CFLAGS = -ansi -pedantic-errors -Wall -Wextra -g -I../$(NAME)/inc
RELEASE_CFLAGS = -ansi -pedantic-errors -Wall -Wextra -DNDEBUG -O3 -I../$(NAME)/inc
CFLAGS = $(DEBUG_CFLAGS)

TARGET = a.out

SRC_DIR = ../$(NAME)/src
TEST_DIR = ../$(NAME)/test

SRC = $(SRC_DIR)/$(NAME).c $(TEST_DIR)/$(NAME)_test.c
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

