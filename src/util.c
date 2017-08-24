#include "emulator.h"
#include <string.h>

uint8_t zero[5] = {0xf0, 0x90, 0x90, 0x90, 0xf0};
uint8_t one[5] = {0x20, 0x60, 0x20, 0x20, 0x70};
uint8_t two[5] = {0xf0, 0x10, 0xf0, 0x80, 0xf0};

void reset(Cpu *cpu) {
  cpu->stack->sp = 0;
  cpu->pc = 0;
  cpu->delay_timer = 0;
  cpu->sound_timer = 0;
  cpu->reg->I = 0;
  // reset registers too
}

void load_fonts(Cpu *cpu) {
  memcpy(cpu->mem->ar, zero, 5);
  memcpy(cpu->mem->ar+5, one, 5);
  memcpy(cpu->mem->ar+10, two, 5);
}

Cpu *initialize() {
  int RAM_SIZE = 4096;
  int REG_SIZE = 16;
  int STACK_SIZE = 16;
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

Screen *initialize_screen() {
  int SCREEN_BYTES = 64 * 32;
  uint8_t *ar = (uint8_t *)malloc(SCREEN_BYTES * sizeof(uint8_t));
  Screen *scr = (Screen *)malloc(sizeof(Screen));
  scr->ar = ar;
  int i;
  for(i=0; i<SCREEN_BYTES; i++) {
    ar[i] = 0x0;
  }
  return scr;
}

Screen *set_pix(Screen *scr, uint8_t pix, uint8_t pix_x, uint8_t pix_y) {
  uint32_t idx = pix_y*64 + pix_x;
  scr->ar[idx] = pix;
  // TODO: add colision detection here
  return scr;
}

void print_cpu(Cpu *cpu) {
  int i, j;
  printf("** Memory **\n");
  for (i=0; i<3; i++) {
    for (j=0; j<8; j++) {
      printf(" 0x%02x ", cpu->mem->ar[i*8 + j]);
      if (j != 7)
        printf("|");
    }
    printf("\n");
  }
}
