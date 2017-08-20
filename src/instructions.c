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

// should put some bounds checking on this...
void stack_push(Stack *stack, uint16_t v) {
  (stack->sp)++;
  stack->ar[stack->sp] = v;
}

uint16_t stack_top(Stack *stack) {
  return stack->ar[stack->sp];
}

/*
  Chip-8 instructions - encoded as functions
 */
/*
  op_code - 00ee
  Return from subroutine. Pop addr from stack, set pc to addr.
*/
int32_t RET(Cpu *cpu) {
  uint16_t top = cpu->stack->ar[cpu->stack->sp];
  (cpu->stack->sp)--;
  cpu->pc = top;
  return 1;
}

/*
  op_code - 1nnn
  Jump. Set pc to nnn.
*/
int32_t JMP(Cpu *cpu, uint16_t o) {
  uint16_t next_instr = o & 0xfff;
  cpu->pc = next_instr;
  return 1;
}

/*
  op_code - 2nnn
  Call subroutine. Increment sp, put current pc on stack, set pc to nnn.
*/
int32_t CALL(Cpu *cpu, uint16_t o) {
  uint16_t subrout_start = o & 0xfff;
  (cpu->stack->sp)++;
  cpu->stack->ar[cpu->stack->sp] = cpu->pc;
  cpu->pc = subrout_start;
  return 1;
}

/*
  op_code - 3xkk
  Skip if regiser Vx equal to kk. If Vx == kk, increment pc by 2.
 */
int32_t SEVx(Cpu *cpu, uint16_t o) {
  uint8_t x = (o & 0xf00) >> 8;
  uint8_t kk = o & 0xff;
  uint8_t vx = cpu->reg->ar[x];
  if (vx == kk) {
    cpu->pc += 2;
  }
  return 1;
}

/*
  This executes an op code and changes the cpu state.
*/
int32_t execute_op_code(Cpu *cpu, opcode o) {
  return 1;
}
