CC := gcc
DEBUG := -ansi -pedantic-errors -Wall -Wextra -g
RELEASE := -ansi -pedantic-errors -Wall -Wextra -DNDEBUG -O3
CFLAGS := $(DEBUG)

DS_INC := ../ds/inc
DS_SRC := \
	../ds/src/pq.c \
	../ds/src/uid.c \
	../ds/src/sortedl.c \
	../ds/src/dll.c

# Build sched.out from sched/src and sched/test/sched_test.c
sched.out: sched/src/sched.c sched/src/task.c sched/test/sched_test.c $(DS_SRC)
	@echo "Building sched.out..."
	$(CC) $(CFLAGS) -Isched/inc -I$(DS_INC) $^ -o $@

# Optional: fsa.out
fsa.out: fsa/src/fsa.c fsa/test/fsa_test.c $(DS_SRC)
	@echo "Building fsa.out..."
	$(CC) $(CFLAGS) -Ifsa/inc -I$(DS_INC) $^ -o $@

# Optional: vsa.out
vsa.out: vsa/src/vsa.c vsa/test/vsa_test.c $(DS_SRC)
	@echo "Building vsa.out..."
	$(CC) $(CFLAGS) -Ivsa/inc -I$(DS_INC) $^ -o $@

task.out: sched/src/sched.c sched/src/task.c sched/test/task_test.c $(DS_SRC)
	@echo "Building task.out..."
	$(CC) $(CFLAGS) -Isched/inc -I$(DS_INC) $^ -o $@

all: sched.out fsa.out vsa.out

debug: CFLAGS := $(DEBUG)
debug: clean all

release: CFLAGS := $(RELEASE)
release: clean all

clean:
	find . -name "*.out" -o -name "*.o" | xargs rm -f

.PHONY: all clean debug release

