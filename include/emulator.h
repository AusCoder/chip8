/*
 * emulator.h
 */

#ifndef _EMULATOR_H_
#define _EMULATOR_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

typedef struct {
  uint8_t *ar;
} Memory;

typedef struct {
  uint8_t *ar;
  uint16_t I;
} Registers;

typedef struct {
  uint16_t *ar;
  uint8_t sp;
} Stack;

typedef struct {
  Memory *mem; // 4096 bytes of memory
  Registers *reg; // 16 8-bit registers
  Stack *stack; // 16 16-bit stack
  uint16_t pc; // 16-bit program counter
  uint16_t delay_timer; // delay timer
  uint16_t sound_timer; // sound timer
} Cpu;

/*
  Clock logic:
    - this opcode takes x cycles
    - execute(Memory + Registers, opcode): (New memory + Registers) and the number of cycles taken
  have a loop:
  while True:
      cycles_taken = execute                 // assume this is instant for a sec
      sleep(cycles_token * time_per_cycle)   // we know time_per_cycle is ~1.7Mhz
 */
typedef struct {
  uint32_t cycles;
  uint32_t time_per_cycle;
} Clock;

typedef struct {
  uint8_t *ar;
} Screen;

typedef uint16_t address;

typedef uint16_t opcode;

// helper cpu functions
void reset(Cpu *);
Cpu *initialize();
Screen *initialize_screen();
Screen *set_pix(Screen *, uint8_t, uint8_t, uint8_t);
void print_cpu(Cpu *);

// memory functions
uint8_t store(Memory *mem, address addr, uint8_t val);
uint8_t load(Memory *mem, address addr);
void stack_push(Stack *, uint16_t);
uint16_t stack_pop(Stack *);
uint16_t stack_top(Stack *);

// execute functions
int32_t execute_op_code(Cpu *, Screen *, opcode);

// instructions, these could go in a different header file
int32_t RET(Cpu *);
int32_t JMP(Cpu *, uint16_t);
int32_t CALL(Cpu *, uint16_t);
int32_t SEVx(Cpu *, uint16_t);
int32_t SNEVx(Cpu *, uint16_t);
int32_t LDVx(Cpu *, uint16_t);
int32_t DRW(Cpu *, Screen *, uint16_t);

// ncurses functions
int draw_screen(Screen *);
void ncurses_emulator(Cpu *, Screen *);

#endif
