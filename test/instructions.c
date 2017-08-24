#include "emulator.h"


int RET_test(Cpu *cpu) {
  reset(cpu);
  stack_push(cpu->stack, 0x1234);
  RET(cpu);
  if (cpu->stack->sp != 0) {
    return 1;
  }
  if (cpu->pc != 0x1234) {
    return 1;
  }
  return 0;
}

int JMP_test(Cpu *cpu) {
  reset(cpu);
  cpu->pc = 0x14;
  JMP(cpu, 0x1576);
  if (cpu->pc != 0x576) {
    return 1;
  }
  return 0;
}

int CALL_test(Cpu *cpu) {
  reset(cpu);
  cpu->pc = 0x634;
  stack_push(cpu->stack, 0x0);
  stack_push(cpu->stack, 0x0);
  stack_push(cpu->stack, 0x0);
  uint8_t prev_sp = cpu->stack->sp;
  CALL(cpu, 0x2583);
  if (cpu->stack->sp != prev_sp + 1)
    return 1;
  if (stack_top(cpu->stack) != 0x634 + 0x2)
    return 1;
  if (cpu->pc != 0x583)
    return 1;
  return 0;
}

int SEVx_test(Cpu *cpu) {
  reset(cpu);
  cpu->pc = 0x857;
  uint8_t i;
  for (i=0; i<16; i++) {
    uint16_t prev_pc = cpu->pc + 2;
    cpu->reg->ar[i] = 0x67;
    SEVx(cpu, 0x3067 | (i << 8));
    if (cpu->pc != prev_pc + 2) {
      return 1;
    }
  }
  for (i=0; i<16; i++) {
    uint16_t prev_pc = cpu->pc + 2;
    cpu->reg->ar[i] = 0x68;
    SEVx(cpu, 0x3067 | (i << 8));
    if (cpu->pc != prev_pc) {
      return 1;
    }
  }
  return 0;
}

int SNEVx_test(Cpu *cpu) {
  reset(cpu);
  cpu->pc = 0xf04;
  uint8_t i;
  for (i=0; i<16; i++) {
    uint16_t prev_pc = cpu->pc + 2;
    cpu->reg->ar[i] = 0x67;
    SNEVx(cpu, 0x3067 | (i << 8));
    if (cpu->pc != prev_pc) {
      return 1;
    }
  }
  for (i=0; i<16; i++) {
    uint16_t prev_pc = cpu->pc + 2;
    cpu->reg->ar[i] = 0x68;
    SNEVx(cpu, 0x3067 | (i << 8));
    if (cpu->pc != prev_pc + 2) {
      return 1;
    }
  }
  return 0;
}

void print_result(int res, char *name) {
  if (res == 0) {
    printf("* %s test passed.\n", name);
  } else {
    printf("*** %s test failed. Printing cpu state:\n", name);
  }
}

int main(int argc, char **argv) {
  Cpu *cpu = initialize();
  printf("Running tests.\n");

  print_result(RET_test(cpu), "RET");
  print_result(JMP_test(cpu), "JMP");
  print_result(CALL_test(cpu), "CALL");
  print_result(SEVx_test(cpu), "SEVx");
  print_result(SNEVx_test(cpu), "SNEVx");

  printf("Tests completed.\n");
}
