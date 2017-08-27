#include "emulator.h"
#include "ncurses_io.h"

// ncurses graphics, these could go in a different header file
uint8_t CHIP8_SCR_SIZE_X = 64;
uint8_t CHIP8_SCR_SIZE_Y = 32;

void ncurses_emulator(Cpu *cpu, Screen *scr) {
  initscr();
  /* draw_sprite(5, cpu->mem->ar+10, 0, 0); */
  draw_screen(scr);
  refresh();
  sleep(1);
  endwin();
}

/*
  Chip8 sprites are represented as a sequence of bytes.
 */

/*
  Draws a sprite.
  n bytes long starting at b_ptr, at position (x, y)
 */
int draw_screen(Screen *scr) {
  int r, c;
  int x = 0;
  int y = 0;
  for(r=0; r<32; r++) {
    int pix_y = y+r;
    for(c=0; c<64; c++) {
      int pix_x = x+c;
      int idx = r*64+c;
      if (scr->ar[idx] & 0x1)
        mvprintw(pix_y, pix_x, "*");
      else
        mvprintw(pix_y, pix_x, " ");
    }
  }
}