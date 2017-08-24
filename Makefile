CC = clang
IDIR=include
SDIR=src
BDIR=build
TDIR=test
TBDIR=build_test
INCLUDES = -I$(IDIR)

CFLAGS = -g -Wall $(INCLUDES)
LDLIBS = -lncurses
LDFLAGS = -g


src = $(shell find $(SDIR) -name *.c)
tsrc = $(shell find $(TDIR) -name *.c)
obj = $(subst $(SDIR),$(BDIR),$(src:.c=.o))
tobj = $(filter-out $(BDIR)/emulator.o,$(obj)) $(subst $(TDIR),$(TBDIR),$(tsrc:.c=.o))

emulator: $(obj)
				$(CC) $(obj) -o $@ $(LDFLAGS) $(LDLIBS)

instructions_test: $(tobj)
				$(CC) $(tobj) -o $@ $(LDFLAGS)

$(BDIR)/%.o: $(SDIR)/%.c
				$(CC) $(CFLAGS) -c -o $@ $<

$(TBDIR)/%.o: $(TDIR)/%.c
				$(CC) $(CFLAGS) -c -o $@ $<

.PHONY: clean
clean:
				rm -f $(obj) $(tobj) a.out core emulator

