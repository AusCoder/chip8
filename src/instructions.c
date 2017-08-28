#include "emulator.h"

uint8_t MAX_VAL = (uint8_t)0xffff;

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

/*
  Stack operations.
 */
void stack_push(Stack *stack, uint16_t v) {
  (stack->sp)++;
  stack->ar[stack->sp] = v;
}

uint16_t stack_pop(Stack *stack) {
  uint16_t top = stack->ar[stack->sp];
  (stack->sp)--;
  return top;
}

uint16_t stack_top(Stack *stack) {
  return stack->ar[stack->sp];
}

/*
  Chip-8 instructions - encoded as functions
 */

/*
  op_code - 00e0
  Clear the screen.
*/
int32_t CLS(Screen *scr) {
  clear_screen(scr);
  return 1;
}

/*
  op_code - 00ee
  Return from subroutine. Pop addr from stack, set pc to addr.
*/
int32_t RET(Cpu *cpu) {
  cpu->pc = stack_pop(cpu->stack);
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
  stack_push(cpu->stack, cpu->pc + 2); // TODO: is add 2 correct here?
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
  cpu->pc += 2;
  return 1;
}

/*
  op_code - 4xkk
  Skip if regiser Vx not equal to kk. If Vx != kk, increment pc by 2.
*/
int32_t SNEVx(Cpu *cpu, uint16_t o) {
  uint8_t x = (o & 0xf00) >> 8;
  uint8_t kk = o & 0xff;
  uint8_t vx = cpu->reg->ar[x];
  if (vx != kk) {
    cpu->pc += 2;
  }
  cpu->pc += 2;
  return 1;
}

/*
  op_code - 5xy0
  Skip if Vx == Vy. If Vx == Vy, increment pc by 2.
*/
int32_t SEVxVy(Cpu *cpu, uint16_t o) {
  uint8_t x = (o & 0xf00) >> 8;
  uint8_t y = (o & 0xf0) >> 4;
  if (cpu->reg->ar[x] == cpu->reg->ar[y]) {
    cpu->pc += 2;
  }
  cpu->pc += 2;
  return 1;
}

/*
  op_code - 6xkk
  Set Vx = kk.
*/
int32_t LDVx(Cpu *cpu, uint16_t o) {
  uint8_t x = (o & 0xf00) >> 8;
  uint8_t kk = o & 0xff;
  cpu->reg->ar[x] = kk;
  cpu->pc += 2;
  return 1;
}

/*
  op_code - 7xkk
  Set Vx = Vx + kk.
*/
int32_t ADDVxKK(Cpu *cpu, uint16_t o) {
  uint8_t x = (o & 0xf00) >> 8;
  uint8_t kk = o & 0xff;
  cpu->reg->ar[x] += kk;
  cpu->pc += 2;
  return 1;
}

/*
  op_code - 8xy0
  Set Vx = Vy.
*/
int32_t LDVxVy(Cpu *cpu, uint16_t o) {
  uint8_t x = (o & 0xf00) >> 8;
  uint8_t y = (o & 0xf0) >> 4;
  cpu->reg->ar[x] = cpu->reg->ar[y];
  cpu->pc += 2;
  return 1;
}

/*
  op_code - 8xy1
  Set Vx = Vx OR Vy.
*/
int32_t ORVxVy(Cpu *cpu, uint16_t o) {
  uint8_t x = (o & 0xf00) >> 8;
  uint8_t y = (o & 0xf0) >> 4;
  cpu->reg->ar[x] = cpu->reg->ar[x] | cpu->reg->ar[y];
  cpu->pc += 2;
  return 1;
}

/*
  op_code - 8xy2
  Set Vx = Vx AND Vy.
*/
int32_t ANDVxVy(Cpu *cpu, uint16_t o) {
  uint8_t x = (o & 0xf00) >> 8;
  uint8_t y = (o & 0xf0) >> 4;
  cpu->reg->ar[x] = cpu->reg->ar[x] & cpu->reg->ar[y];
  cpu->pc += 2;
  return 1;
}

/*
  op_code - 8xy3
  Set Vx = Vx XOR Vy.
*/
int32_t XORVxVy(Cpu *cpu, uint16_t o) {
  uint8_t x = (o & 0xf00) >> 8;
  uint8_t y = (o & 0xf0) >> 4;
  cpu->reg->ar[x] = cpu->reg->ar[x] ^ cpu->reg->ar[y];
  cpu->pc += 2;
  return 1;
}

/*
  op_code - 8xy4
  Set Vx = Vx + Vy. Set Vf = carry.
*/
int32_t ADDVxVy(Cpu *cpu, uint16_t o) {
  uint8_t x = (o & 0xf00) >> 8;
  uint8_t y = (o & 0xf0) >> 4;
  uint8_t vx = cpu->reg->ar[x];
  uint8_t vy = cpu->reg->ar[y];
  cpu->reg->ar[x] = vx + vy;
  if (x > MAX_VAL - y) {
    cpu->reg->ar[0xf] = 1;
  } else {
    cpu->reg->ar[0xf] = 0;
  }
  cpu->pc += 2;
  return 1;
}

/*
  op_code - 8xy5
  Set Vx = Vx - Vy. If Vx > Vy, set Vf = 1, else Vf = 0.
*/
int32_t SUBVxVy(Cpu *cpu, uint16_t o) {
  uint8_t x = (o & 0xf00) >> 8;
  uint8_t y = (o & 0xf0) >> 4;
  uint8_t vx = cpu->reg->ar[x];
  uint8_t vy = cpu->reg->ar[y];
  cpu->reg->ar[x] = vx - vy;
  if (vx > vy) {
    cpu->reg->ar[0xf] = 1;
  } else {
    cpu->reg->ar[0xf] = 0;
  }
  cpu->pc += 2;
  return 1;
}

/*
  op_code - 8xy6
  Logical shift right. Set Vx = Vx SHR 1, set Vf = 1 if lsb of Vx is 1, else Vf = 0.
*/
int32_t SHRVx(Cpu *cpu, uint16_t o) {
  uint8_t x = (o & 0xf00) >> 8;
  uint8_t vx = cpu->reg->ar[x];
  cpu->reg->ar[x] = vx >> 1;
  cpu->reg->ar[0xf] = vx & 0x1; // TODO: is this right?
  cpu->pc += 2;
  return 1;
}

/*
  op_code - 8xy7
  Set Vx = Vy - Vx. If Vy > Vx, Vf = 1 else Vf = 0.
*/
int32_t SUBNVxVy(Cpu *cpu, uint16_t o) {
  uint8_t x = (o & 0xf00) >> 8;
  uint8_t y = (o & 0xf0) >> 4;
  uint8_t vx = cpu->reg->ar[x];
  uint8_t vy = cpu->reg->ar[y];
  cpu->reg->ar[x] = vy - vx;
  if (vy > vx) {
    cpu->reg->ar[0xf] = 0x1;
  } else {
    cpu->reg->ar[0xf] = 0x0;
  }
  cpu->pc += 2;
  return 1;
}

/*
  op_code - 8xye
  Logical shift left. Set Vx = Vy SHL 1, set Vf = 1 if msb of Vx is 1, else Vf = 0.
*/
int32_t SHLVx(Cpu *cpu, uint16_t o) {
  uint8_t x = (o & 0xf00) >> 8;
  uint8_t vx = cpu->reg->ar[x];
  cpu->reg->ar[x] = vx << 1;
  cpu->reg->ar[0xf] = vx & 0x80; // TODO: is this right?
  cpu->pc += 2;
  return 1;
}

/*
  op_code - 9xy0
  Skip next instruction if Vx != Vy.
*/
int32_t SNEVxVy(Cpu *cpu, uint16_t o) {
  uint8_t x = (o & 0xf00) >> 8;
  uint8_t y = (o & 0xf0) >> 4;
  uint8_t vx = cpu->reg->ar[x];
  uint8_t vy = cpu->reg->ar[y];
  if (vx != vy) {
    cpu->pc += 2;
  }
  cpu->pc += 2;
  return 1;
}

/*
  op_code - annnn
  Set register I = nnn.
*/
int32_t SetI(Cpu *cpu, uint16_t o) {
  uint16_t nnn = o & 0xfff;
  cpu->reg->I = nnn;
  cpu->pc += 2;
  return 1;
}

/*
  op_code - bnnn
  Jump to location nnn + V0. Set pc to V0 + nnn.
*/
int32_t JMPV0(Cpu *cpu, uint16_t o) {
  uint16_t nnn = o & 0xfff;
  uint16_t v0 = (uint16_t)cpu->reg->ar[0x0];
  cpu->pc = nnn + v0;
  return 1;
}

/*
  op_code - cxkk
  Set Vx = RAND_BYTE & 0xkk.
*/
int32_t RANDVx(Cpu *cpu, uint16_t o) {
  uint8_t x = (o & 0xf00) >> 8;
  uint8_t kk = o & 0xff;
  uint8_t r = (uint8_t)rand(); // TODO: change this random number
  cpu->reg->ar[x] = kk & r;
  cpu->pc += 2;
  return 1;
}

/*
  op_code - dxyn
  Display n byte sprite starting at memory location I at (Vx, Vy), set Vf = collision.
*/
int32_t DRW(Cpu *cpu, Screen *scr, uint16_t o) {
  uint8_t x = (o & 0xf00) >> 8;
  uint8_t y = (o & 0xf0) >> 4;
  uint8_t vx = cpu->reg->ar[x];
  uint8_t vy = cpu->reg->ar[y];
  uint8_t n = o & 0xf;
  uint8_t i;
  for(i=0; i<n; i++) {
    uint8_t img_byte = load(cpu->mem, cpu->reg->I + i);
    int c;
    for(c=0; c<8; c++) {
      uint8_t pix = (img_byte >> (7-c)) & 0x1;
      uint8_t pix_x = vx+c;
      uint8_t pix_y = vy+i;
      set_pix(scr, pix, pix_x, pix_y); // TODO: need to add collision detection
    }
  }
  cpu->pc+=2;
  return 1;
}

/*
  op_code - ex9e
  Skip next instruction if key with value of Vx is pressed.
*/
// TODO: Do this keyboard opcode.
int32_t SKPVx(Cpu *cpu, uint16_t o) {
  uint8_t x = (o & 0xf00) >> 8;

  cpu->pc += 2;
  return 1;
}

/*
  op_code - exa1
  Skip next instruction if key with value of Vx is not pressed.
*/
// TODO: Do this keyboard opcode.
int32_t SKNPVx(Cpu *cpu, uint16_t o) {
  uint8_t x = (o & 0xf00) >> 8;

  cpu->pc += 2;
  return 1;
}

/*
  op_code - fx07
  Set Vx = delay timer value.
*/
int32_t LDVxDT(Cpu *cpu, uint16_t o) {
  uint8_t x = (o & 0xf00) >> 8;
  cpu->reg->ar[x] = cpu->delay_timer;
  cpu->pc+=2;
  return 1;
}

/*
  op_code - fx0a
  Wait for a key press, store it in Vx.
*/
// TODO: Do this keyboard opcode. need to add blocking.
int32_t LDVxK(Cpu *cpu, uint16_t o) {
  uint8_t x = (o & 0xf00) >> 8;

  cpu->pc += 2;
  return 1;
}

/*
  op_code - fx15
  Set delay_timer = Vx.
*/
int32_t LDDTVx(Cpu *cpu, uint16_t o) {
  uint8_t x = (o & 0xf00) >> 8;
  cpu->delay_timer = cpu->reg->ar[x];
  cpu->pc+=2;
  return 1;
}

/*
  op_code - fx18
  Set sound_timer = Vx.
*/
int32_t LDSTVx(Cpu *cpu, uint16_t o) {
  uint8_t x = (o & 0xf00) >> 8;
  cpu->sound_timer = cpu->reg->ar[x];
  cpu->pc+=2;
  return 1;
}

/*
  op_code - fx1e
  Set I = I + Vx.
*/
int32_t ADDIVx(Cpu *cpu, uint16_t o) {
  uint8_t x = (o & 0xf00) >> 8;
  cpu->reg->I += cpu->reg->ar[x];
  cpu->pc+=2;
  return 1;
}

/*
  op_code - fx29
  Set I = location of sprite for digit Vx.
*/
int32_t LDFVx(Cpu *cpu, uint16_t o) {
  uint8_t x = (o & 0xf00) >> 8;
  // digits are sequential, of size 5 bytes, starting at 0x0
  cpu->reg->I = cpu->reg->ar[x] * 5;
  cpu->pc+=2;
  return 1;
}

/*
  op_code - fx33
  Take decimal value of Vx. Store hundreds in I, tens in I+1, uints in I+2.
*/
int32_t LDBVx(Cpu *cpu, uint16_t o) {
  uint8_t x = (o & 0xf00) >> 8;
  uint16_t vx = cpu->reg->ar[x];
  address i = cpu->reg->I;
  store(cpu->mem, i, (vx % 1000) / 100);
  store(cpu->mem, i+1, (vx % 100) / 10);
  store(cpu->mem, i+2, vx % 10);
  cpu->pc += 2;
  return 1;
}

/*
  op_code - fx55
  Store registers V0 to Vx in memory, starting at I.
*/
int32_t LDIVx(Cpu *cpu, uint16_t o) {
  uint8_t x = (o & 0xf00) >> 8;
  address i = cpu->reg->I;
  uint8_t j;
  for(j=0; j<=x; j++) {
    store(cpu->mem, i+j, cpu->reg->ar[j]); // TODO: check this, does an offset need to be respected?
  }
  cpu->pc += 2;
  return 1;
}

/*
  op_code - fx65
  Read values from memory location I into registers V0 to Vx.
*/
int32_t LDVxI(Cpu *cpu, uint16_t o) {
  uint8_t x = (o & 0xf00) >> 8;
  uint16_t i = cpu->reg->I;
  uint8_t j;
  for(j=0; j<=x; j++) {
    cpu->reg->ar[j] = load(cpu->mem, i+j);
  }
  cpu->pc += 2;
  return 1;
}

/*
  This executes an op code and changes the cpu state.
*/
int32_t execute_op_code(Cpu *cpu, Screen *scr, uint16_t o) {
  int32_t i = 0;
  switch (o & 0xf000) {
  case 0x0000: {
    switch (0 & 0xff) {
    case 0xe0:
      i = CLS(scr);
      break;
    case 0xee:
      i = RET(cpu);
      break;
    }
    break;
  }
  case 0x1000:
    i = JMP(cpu, o);
    break;
  case 0x2000:
    i = CALL(cpu, o);
    break;
  case 0x3000:
    i = SEVx(cpu, o);
    break;
  case 0x4000:
    i = SNEVx(cpu, o);
    break;
  case 0x5000:
    i = SEVxVy(cpu, o);
    break;
  case 0x6000:
    i = LDVx(cpu, o);
    break;
  case 0x7000:
    i = ADDVxKK(cpu, o);
    break;
  case 0x8000: {
    switch (o & 0xf) {
    case 0x0:
      i = LDVxVy(cpu, o);
      break;
    case 0x1:
      i = ORVxVy(cpu, o);
      break;
    case 0x2:
      i = ANDVxVy(cpu, o);
      break;
    case 0x3:
      i = XORVxVy(cpu, o);
      break;
    case 0x4:
      i = ANDVxVy(cpu, o);
      break;
    case 0x5:
      i = SUBVxVy(cpu, o);
      break;
    case 0x6:
      i = SHRVx(cpu, o);
      break;
    case 0x7:
      i = SUBNVxVy(cpu, o);
      break;
    case 0xe:
      i = SHLVx(cpu, o);
      break;
    }
    break;
  }
  case 0x9000:
    i = SNEVxVy(cpu, o);
    break;
  case 0xa000:
    i = SetI(cpu, o);
    break;
  case 0xb000:
    i = JMPV0(cpu, o);
    break;
  case 0xc000:
    i = RANDVx(cpu, o);
    break;
  case 0xd000:
    i = DRW(cpu, scr, o);
    break;
  case 0xe000: {
    switch (o & 0xff) {
    case 0x93:
      i = SKNPVx(cpu, o);
      break;
    case 0xa1:
      i = SKNPVx(cpu, o);
      break;
    }
    break;
    }
  case 0xf000:
    switch (o & 0xff) {
    case 0x07:
      i = LDVxDT(cpu, o);
      break;
    case 0x0a:
      i = LDVxK(cpu, o);
      break;
    case 0x15:
      i = LDDTVx(cpu, o);
      break;
    case 0x18:
      i = LDSTVx(cpu, o);
      break;
    case 0x1e:
      i = ADDIVx(cpu, o);
      break;
    case 0x29:
      i = LDFVx(cpu, o);
      break;
    case 0x33:
      i = LDBVx(cpu, o);
      break;
    case 0x55:
      i = LDIVx(cpu, o);
      break;
    case 0x65:
      i = LDVxI(cpu, o);
      break;
    }
    break;
  }
  return i;
}
