/*
 * emulator.h
 */

#ifndef _EMULATOR_H_
#define _EMULATOR_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <unistd.h>


const int RAM_SIZE;
const int REG_SIZE;
const int STACK_SIZE;
const int SCREEN_WIDTH;
const int SCREEN_HEIGHT;
const int KEYBOARD_SIZE;


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
  Memory *mem;           // 4096 bytes of memory
  Registers *reg;        // 16 8-bit registers
  Stack *stack;          // 16 16-bit stack
  uint16_t pc;           // 16-bit program counter
  uint16_t delay_timer;  // delay timer
  uint16_t sound_timer;  // sound timer
} Cpu;

// FIXME: add a changed bool here, so that we only have to redraw the screen if something changed! That would be fun.
typedef struct {
  uint8_t *ar;           // a 64 * 32 array of bytes to represent the screen
} Screen;

/*
  Chip8 keyboard layout

  1 2 3 C
  4 5 6 D
  7 8 9 E
  A 0 B F
 */
typedef struct {
  uint8_t *ar;           // a 16 array to represent the keyboard
} Keyboard;              // a 0x0 represents up, 0x1 represents down

const int CHIP8_0;  // mapping of keys to array index
const int CHIP8_1;  // QUESTION: can I define these in a header?
const int CHIP8_2;
const int CHIP8_3;
const int CHIP8_4;
const int CHIP8_5;
const int CHIP8_6;
const int CHIP8_7;
const int CHIP8_8;
const int CHIP8_9;
const int CHIP8_a;
const int CHIP8_b;
const int CHIP8_c;
const int CHIP8_d;
const int CHIP8_e;
const int CHIP8_f;

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

typedef uint16_t address;
typedef uint16_t opcode;
// FIXME: add typedefs for: memval, regval, stkval, keyval

/* Function that actually execute an op code */
int32_t execute_op_code(Cpu *, Screen *, Keyboard *, opcode);

/* Helper functions for cpu and screen */
Cpu *initialize_cpu();
void destroy_cpu(Cpu *);
void reset(Cpu *);
Screen *initialize_screen();
void destroy_screen(Screen *);
int set_pix(Screen *, uint8_t, uint8_t, uint8_t);
void clear_screen(Screen *);
Keyboard *initialize_keyboard();
void log_emulator_error(const char *);
void print_cpu(Cpu *);
int load_rom(Cpu *, const char *);

/* Blocking keyboard input */
int blocking_keyboard_read();

/* Helper functions to accessing memory and stack */
uint8_t store(Memory *mem, address addr, uint8_t val);
uint8_t load(Memory *mem, address addr);
void stack_push(Stack *, uint16_t);
uint16_t stack_pop(Stack *);
uint16_t stack_top(Stack *);

/* Op code functions - these modify Cpu state */
int32_t RET(Cpu *);
int32_t JMP(Cpu *, uint16_t);
int32_t CALL(Cpu *, uint16_t);
int32_t SEVx(Cpu *, uint16_t);
int32_t SNEVx(Cpu *, uint16_t);
int32_t LDVx(Cpu *, uint16_t);
int32_t DRW(Cpu *, Screen *, uint16_t);

#endif
