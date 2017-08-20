#include "emulator.h"


int RET_test(Cpu *cpu) {
  reset(cpu);

  stack_push(cpu->stack, 0x1234);
  RET(cpu);
  if (cpu->stack->sp != 0) {
    printf("RET test failed, sp is %d.\n", cpu->stack->sp);
    return 1;
  }
  if (cpu->pc != 0x1234) {
    printf("RET test failed, pc is %d.\n", cpu->pc);
    return 1;
  }
  printf("RET test passed.\n");
  return 0;
}


int main(int argc, char **argv) {
  Cpu *cpu = initialize();
  printf("Running tests.\n");
  int res;
  RET_test(cpu);
  printf("Tests completed.\n");
}
