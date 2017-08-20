CC = clang
IDIR=include
SDIR=src
TDIR=test
BDIR=build
INCLUDES = -I$(IDIR)

CFLAGS = -g -Wall $(INCLUDES)
LDFLAGS = -g

LDLIBS =

src = $(shell find $(SDIR) -name *.c)
tsrc = $(shell find $(TDIR) -name *.c)
obj = $(subst $(SDIR),$(BDIR),$(src:.c=.o))
tobj = $(subst $(TDIR),$(BDIR),$(tsrc:.c=.o))

emulator: $(obj)
				$(CC) $(obj) -o $@ $(LDFLAGS)

$(BDIR)/%.o: $(SDIR)/%.c
				$(CC) $(CFLAGS) -c -o $@ $<

# emulator: emulator.o

# emulator.o:
# 				clang src/emulator.c -

.PHONY: clean
clean:
				rm -f $(obj) a.out core emulator

