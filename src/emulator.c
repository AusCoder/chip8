#include "emulator.h" // how to fix this red?

/*
    Questions:
    how to represent bytes? use uint8_t defined in stdint.h
    how can I represent a 2 byte thing? uint16_t from stdint.h
 */

void run(Cpu *cpu) {
  for (;;) {
    // start timer
    address addr_nxt_instr = cpu->pc;
    uint8_t msb = load(cpu->mem, addr_nxt_instr);
    uint8_t lsb = load(cpu->mem, addr_nxt_instr + 1);
    opcode op_code = msb << 8 | lsb;
    /* int32_t cycles = execute_op_code(cpu, op_code); */
    /* if (cycles < 0) { */
    /*   printf("An error occured executing op_code: %d.", cycles); */
      /* sys.exit(1); */
    /* }; */
    // end timer, sleep for: cycles * clock_rate - time_taken
  };
}

int main(int argc, char **argv) {
  printf("Starting Emulator.\n");
  Cpu *cpu = initialize();
  uint8_t z = (0x1a00 & 0xf00) >> 8;
  store(cpu->mem, 79, 76);
  uint8_t t = load(cpu->mem, 79);
  printf("%d.\n", z);
  return 0;
}
