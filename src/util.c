#include "emulator.h"

void reset(Cpu *cpu) {
  cpu->stack->sp = 0;
  cpu->pc = 0;
  cpu->delay_timer = 0;
  cpu->sound_timer = 0;
  // reset registers too
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
  // allocate stack
  uint16_t *st_ar = (uint16_t *)malloc(STACK_SIZE * sizeof(uint16_t));
  Stack *stack = (Stack *)malloc(sizeof(Stack));
  stack->ar = st_ar;
  // allocate Cpu
  Cpu *cpu = (Cpu *)malloc(sizeof(Cpu));
  cpu->mem = mem;
  cpu->reg = reg;
  cpu->stack = stack;
  reset(cpu);
  return cpu;
}

void print_cpu() {
  
}