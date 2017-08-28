#include "emulator.h" // how to fix this red?
#include "ncurses_io.h"

/*
    Questions:
    how to represent bytes? use uint8_t defined in stdint.h
    how can I represent a 2 byte thing? uint16_t from stdint.h
 */

void run(Cpu *cpu, Screen *scr) {
  for (;;) {
    // start timer
    address addr_nxt_instr = cpu->pc;
    uint8_t msb = load(cpu->mem, addr_nxt_instr);
    uint8_t lsb = load(cpu->mem, addr_nxt_instr + 1);
    opcode op_code = msb << 8 | lsb;
    int32_t cycles = execute_op_code(cpu, scr, op_code);
    draw_screen(scr);
    refresh();
    usleep(2000);
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
  Screen *scr = initialize_screen();
  load_rom(cpu, "/Users/seb/code/fun/chip8_emulator/data/breakout.ch8");

  print_cpu(cpu);

  // run the rom
  initscr();
  run(cpu, scr);
  endwin();

  return 0;
}
