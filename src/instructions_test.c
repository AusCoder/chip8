#include "emulator.h"


int RET_test(Cpu *cpu) {
  reset(cpu);

  stack_push(cpu->stack, 0x1234);
  RET(cpu);
  if (cpu->stack->sp != 0) {
    printf("RET test failed, sp is %d.", cpu->stack->sp);
  }
  if (cpu->stack->ar[cpu->stack->sp] != 0x1234) {
    printf("RET test failed, top is %d.", cpu->stack->ar[cpu->stack->sp]);
  }
  printf("RET test passed.");
}


int main(int argc, char **argv) {
  Cpu *cpu = initialize();
  printf("Running tests.");
  int res;
  RET_test(cpu);
  printf("Tests completed.");
}
