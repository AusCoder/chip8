#include "emulator.h"
#include <string.h>

const int RAM_SIZE = 4096;
const int REG_SIZE = 16;
const int STACK_SIZE = 16;
const int SCREEN_WIDTH = 64;
const int SCREEN_HEIGHT = 32;
const int KEYBOARD_SIZE = 16;

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

Cpu *initialize_cpu() {
  // allocate ram
  uint8_t *ar = (uint8_t *)malloc(RAM_SIZE * sizeof(uint8_t));
  if (ar == NULL) {
    return NULL;
  }
  Memory *mem = (Memory *)malloc(sizeof(Memory));
  if (mem == NULL) {
    return NULL;
  }
  mem->ar = ar;
  // allocate registers
  uint8_t *reg_ar = (uint8_t *)malloc(REG_SIZE * sizeof(uint8_t));
  if (reg_ar == NULL) {
    return NULL;
  }
  Registers *reg = (Registers *)malloc(sizeof(Registers));
  if (reg == NULL) {
    return NULL;
  }
  reg->ar = reg_ar;
  reg->I = 0;
  // allocate stack
  uint16_t *st_ar = (uint16_t *)malloc(STACK_SIZE * sizeof(uint16_t));
  if (st_ar == NULL) {
    return NULL;
  }
  Stack *stack = (Stack *)malloc(sizeof(Stack));
  if (stack == NULL) {
    return NULL;
  }
  stack->ar = st_ar;
  // allocate Cpu
  Cpu *cpu = (Cpu *)malloc(sizeof(Cpu));
  if (cpu == NULL) {
    return NULL;
  }
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

Screen *initialize_screen() {
  uint8_t *ar = (uint8_t *)malloc(SCREEN_WIDTH * SCREEN_HEIGHT * sizeof(uint8_t));
  if (ar == NULL) {
    return NULL;
  }
  Screen *scr = (Screen *)malloc(sizeof(Screen));
  if (scr == NULL) {
    return NULL;
  }
  scr->ar = ar;
  clear_screen(scr);
  return scr;
}

void destroy_screen(Screen *scr) {
  // TODO: free the screen
}

int set_pix(Screen *scr, uint8_t pix, uint8_t pix_x, uint8_t pix_y) {
  uint32_t idx = pix_y*SCREEN_WIDTH + pix_x;
  // TODO: add colision detection here
  /* int out; */
  /* if (scr->ar[]) */
  scr->ar[idx] = pix;
  return 0;
}

void clear_screen(Screen *scr) {
  int i;
  for(i=0; i<(SCREEN_WIDTH * SCREEN_HEIGHT); i++) {
    scr->ar[i] = 0x0;
  }
}

const int CHIP8_0 = 0x0;  // mapping of keys to array index
const int CHIP8_1 = 0x1;  // QUESTION: can I define these in a header?
const int CHIP8_2 = 0x2;
const int CHIP8_3 = 0x3;
const int CHIP8_4 = 0x4;
const int CHIP8_5 = 0x5;
const int CHIP8_6 = 0x6;
const int CHIP8_7 = 0x7;
const int CHIP8_8 = 0x8;
const int CHIP8_9 = 0x9;
const int CHIP8_a = 0xa;
const int CHIP8_b = 0xb;
const int CHIP8_c = 0xc;
const int CHIP8_d = 0xd;
const int CHIP8_e = 0xe;
const int CHIP8_f = 0xf;

Keyboard *initialize_keyboard() {
  uint8_t *ar = (uint8_t *)malloc(KEYBOARD_SIZE * sizeof(uint8_t));
  if (ar == NULL) {
    return NULL;
  }
  Keyboard *keys = (Keyboard *)malloc(sizeof(Keyboard));
  if (keys == NULL) {
    return NULL;
  }
  keys->ar = ar;
  return keys;
}

void log_emulator_error(const char *msg) {
  printf("Emulator Error: %s\n", msg);
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
  printf("** Registers **\n");
  for (i=0; i<REG_SIZE; i++) {
    printf(" 0x%02x |", cpu->reg->ar[i]);
  }
  printf("\n");
  printf("** Others **\n");
  printf(" PC: 0x%04x | Delay Timer: 0x%04x | Sound Timer: 0x%04x\n", cpu->pc, cpu->delay_timer, cpu->sound_timer);
  /* printf("** Stack **\n"); */
  /* printf(" sp: 0x%02x ||", cpu->stack->sp); */
  /* for (i=0; i < 16; i++) { */
  /*   printf(" 0x%02x ", cpu->stack->ar[i]); */
  /* } */
  /* printf("\n"); */
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
