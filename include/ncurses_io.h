
#ifndef _NCURSES_IO_H_
#define _NCURSES_IO_H_

#include <ncurses.h>
#include "emulator.h"

void ncurses_emulator(Cpu *cpu, Screen *scr);
int draw_screen(Screen *scr);

#endif
