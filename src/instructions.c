#include "emulator.h"

/*
  Stores val in address and returns previous value.
*/
uint8_t store(Memory *mem, address addr, uint8_t val) {
  uint8_t *m = mem->ar + addr;
  uint8_t o = *m;
  *m = val;
  return o;
}

uint8_t load(Memory *mem, address addr) {
  return *(mem->ar + addr);
}

void stack_push(Stack *stack, uint16_t v) {
  (stack->sp)++;
  stack->ar[stack->sp] = v;
}

/*
  Chip-8 instructions - encoded as functions
 */
int32_t RET(Cpu *cpu) {
  uint16_t top = cpu->stack->ar[cpu->stack->sp];
  (cpu->stack->sp)--;
  cpu->pc = top;
  return 1;
}

int32_t JMP(Cpu *cpu, uint16_t o) {
  uint16_t next_instr = o & 0xfff;
  cpu->pc = next_instr;
  return 1;
}

int32_t CALL(Cpu *cpu, uint16_t o) {
  uint16_t subrout_start = o & 0xfff;
  (cpu->stack->sp)++;
  cpu->stack->ar[cpu->stack->sp] = cpu->pc;
  cpu->pc = subrout_start;
  return 1;
}

/*
  This executes an op code and changes the cpu state.
*/
int32_t execute_op_code(Cpu *cpu, opcode o) {
  return 1;
}
