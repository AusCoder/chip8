CC = clang
IDIR = include
INCLUDES = -I$(IDIR)

CFLAGS = -g -Wall $(INCLUDES)
LDFLAGS = -g

LDLIBS =

src = $(wildcard src/*.c)
obj = $(src:.c=.o)

emulator: $(obj)
				$(CC) -o $@ $^ $(LDFLAGS)

emulator_test: instructions.o instruction_test.o

# emulator: emulator.o

# emulator.o:
# 				clang src/emulator.c -

.PHONY: clean
clean:
				rm -f $(obj) a.out core emulator

