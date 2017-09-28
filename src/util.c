#include "emulator.h"
#include <string.h>

int RAM_SIZE = 4096;
int REG_SIZE = 16;
int STACK_SIZE = 16;
int SCREEN_WIDTH = 64;
int SCREEN_HEIGHT = 32;

uint8_t zero[5] = {0xf0, 0x90, 0x90, 0x90, 0xf0};
uint8_t one[5] = {0x20, 0x60, 0x20, 0x20, 0x70};
uint8_t two[5] = {0xf0, 0x10, 0xf0, 0x80, 0xf0};
uint8_t three[5] = {0xf0, 0x10, 0xf0, 0x10, 0xf0};
uint8_t four[5] = {0x90, 0x90, 0xf0, 0x10, 0x10};
uint8_t five[5] = {0xf0, 0x80, 0xf0, 0x10, 0xf0};
uint8_t six[5] = {0xf0, 0x80, 0xf0, 0x90, 0xf0};
uint8_t seven[5] = {0xf0, 0x10, 0x20, 0x40, 0x40};
uint8_t eight[5] = {0xf0, 0x90, 0xf0, 0x90, 0xf0};
uint8_t nine[5] = {0xf0, 0x90, 0xf0, 0x10, 0xf0};
uint8_t ten[5] = {0xf0, 0x90, 0xf0, 0x90, 0x90};
uint8_t eleven[5] = {0xe0, 0x90, 0xe0, 0x90, 0xe0};
uint8_t twelve[5] = {0xf0, 0x80, 0x80, 0x80, 0xf0};
uint8_t thirteen[5] = {0xe0, 0x90, 0x90, 0x90, 0xe0};
uint8_t fourteen[5] = {0xf0, 0x80, 0xf0, 0x80, 0xf0};
uint8_t fifteen[5] = {0xf0, 0x80, 0xf0, 0x80, 0x80};

void load_fonts(Cpu *cpu) {
  memcpy(cpu->mem->ar, zero, 5);
  memcpy(cpu->mem->ar+5, one, 5);
  memcpy(cpu->mem->ar+10, two, 5);
  memcpy(cpu->mem->ar+15, three, 5);
  memcpy(cpu->mem->ar+20, four, 5);
  memcpy(cpu->mem->ar+25, five, 5);
  memcpy(cpu->mem->ar+30, six, 5);
  memcpy(cpu->mem->ar+35, seven, 5);
  memcpy(cpu->mem->ar+40, eight, 5);
  memcpy(cpu->mem->ar+45, nine, 5);
  memcpy(cpu->mem->ar+50, ten, 5);
  memcpy(cpu->mem->ar+55, eleven, 5);
  memcpy(cpu->mem->ar+60, twelve, 5);
  memcpy(cpu->mem->ar+65, thirteen, 5);
  memcpy(cpu->mem->ar+70, fourteen, 5);
  memcpy(cpu->mem->ar+75, fifteen, 5);
}

void reset(Cpu *cpu) {
  cpu->stack->sp = 0;
  cpu->pc = 0x200;
  cpu->delay_timer = 0;
  cpu->sound_timer = 0;
  cpu->reg->I = 0;
  int i;
  for (i=0; i<REG_SIZE; i++) {
    cpu->reg->ar[i] = 0;
  }
}

Cpu *initialize_cpu() {
  // allocate ram
  uint8_t *ar = (uint8_t *)malloc(RAM_SIZE * sizeof(uint8_t));
  Memory *mem = (Memory *)malloc(sizeof(Memory));
  mem->ar = ar;
  // allocate registers
  uint8_t *reg_ar = (uint8_t *)malloc(REG_SIZE * sizeof(uint8_t));
  Registers *reg = (Registers *)malloc(sizeof(Registers));
  reg->ar = reg_ar;
  reg->I = 0;
  // allocate stack
  uint16_t *st_ar = (uint16_t *)malloc(STACK_SIZE * sizeof(uint16_t));
  Stack *stack = (Stack *)malloc(sizeof(Stack));
  stack->ar = st_ar;
  // allocate Cpu
  Cpu *cpu = (Cpu *)malloc(sizeof(Cpu));
  cpu->mem = mem;
  cpu->reg = reg;
  cpu->stack = stack;
  load_fonts(cpu);
  reset(cpu);
  return cpu;
}

void destroy_cpu(Cpu *cpu) {
  // TODO: free the cpu
}

Screen *initialize_screen() {
  uint8_t *ar = (uint8_t *)malloc(SCREEN_WIDTH * SCREEN_HEIGHT * sizeof(uint8_t));
  Screen *scr = (Screen *)malloc(sizeof(Screen));
  scr->ar = ar;
  clear_screen(scr);
  return scr;
}

void destroy_screen(Screen *scr) {
  // TODO: free the screen
}

int set_pix(Screen *scr, uint8_t pix, uint8_t pix_x, uint8_t pix_y) {
  uint32_t idx = pix_y*64 + pix_x;
  int out;
  /* if (scr->ar[]) */
  scr->ar[idx] = pix;
  // TODO: add colision detection here
  return 0;
}

void clear_screen(Screen *scr) {
  int i;
  for(i=0; i<(SCREEN_WIDTH * SCREEN_HEIGHT); i++) {
    scr->ar[i] = 0x0;
  }
}

void print_cpu(Cpu *cpu) {
  int start_addr = 0x200;
  int i, j;
  printf("** Memory - from 0x%04x **\n", start_addr);
  for (i=0; i<3; i++) {
    for (j=0; j<8; j++) {
      printf(" 0x%02x ", cpu->mem->ar[start_addr + i*8 + j]);
      if (j != 7)
        printf("|");
    }
    printf("\n");
  }
}

int load_rom(Cpu *cpu, const char *filename) {
  FILE *fp = fopen(filename, "r");
  int CHUNK=1000;
  uint8_t bs[CHUNK];
  int bs_read;
  int offset = 0x200;
  while ((bs_read = fread(bs, sizeof(uint8_t), CHUNK, fp)) != 0) {
    memcpy(cpu->mem->ar + offset, bs, bs_read);
    offset += bs_read;
  }
  fclose(fp);
  return 0;
}

// TODO: write a silly program:
// set vx = 0.
// set I = font location of vx.
// draw digit vx
// increment vx
// repeat

uint16_t *counting_program() {
  uint16_t *prog = (uint16_t *)malloc(100 * sizeof(uint16_t));
  uint16_t i = 0;
  prog[i++] = 0x6000;
  prog[i++] = 0xf029;
  return prog;
}
