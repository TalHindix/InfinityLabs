

#  Toolchain & flags
CC := gcc
GD := -ansi -pedantic-errors -Wall -Wextra -g
GC := -ansi -pedantic-errors -Wall -Wextra -DNDEBUG -O3
CFLAGS := $(GD)
TARGET := fooer.out

OBJS := foo.o main.o

#  PATH


# Default Target

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $(LDFLAGS) $(OBJS) -o $(TARGET)

foo.o: foo.h

main.o: foo.h


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
	
