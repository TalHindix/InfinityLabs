

#  Toolchain & flags
CC := gcc
GD := -ansi -pedantic-errors -Wall -Wextra -g
GC := -ansi -pedantic-errors -Wall -Wextra -DNDEBUG -O3
CFLAGS := $(GD)
TARGET := fooer.out

OBJS := foo.o main.o

#  PATH
SRC_FOO = src/foo.c
SRC_TST = test/main.c
HDR_FOO = include/foo.h

# Default Target

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $(LDFLAGS) $(OBJS) -o $(TARGET)

foo.o: $(SRC_FOO) $(HDR_FOO)
	$(CC) $(CFLAGS) -Iinclude -c $(SRC_FOO) -o foo.o

main.o: $(SRC_TST) $(HDR_FOO)
	$(CC) $(CFLAGS) -Iinclude -c $(SRC_TST) -o main.o


# Helpers

.PHONY: clean debug release

debug:
	$(MAKE) clean
	$(MAKE) CFLAGS="$(GD)"
	
	
release:
	$(MAKE) clean
	$(MAKE) CFLAGS="$(GC)"
	
	
clean:
	rm -f $(OBJS) $(TARGET)
	
