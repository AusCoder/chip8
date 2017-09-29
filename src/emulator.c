#include "emulator.h" // how to fix this red?
#include "sdl_io.h"

int main(int argc, char **argv) {
  printf("Starting Emulator.\n");
  if (argc != 2) {
    printf("Please provide a rom!\n");
    return 1;
  }
  Cpu *cpu = initialize_cpu();
  Screen *scr = initialize_screen();
  Keyboard *keys = initialize_keyboard();
  reset(cpu, scr, keys);
  load_rom(cpu, argv[1]);

  /* Start SDL */
  if (SDL_Init(SDL_INIT_VIDEO) != 0){
    log_sdl_error("SDL_Init");
		return 1;
	}
  SDL_Window *win = SDL_CreateWindow("Chip8", 100, 100, SDL_SCREEN_WIDTH, SDL_SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
  if (win == NULL) {
    log_sdl_error("SDL_CreateWindow");
    SDL_Quit();
    destroy_cpu(cpu);
    return 1;
  }
  SDL_Renderer *ren = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
  if (ren == NULL) {
    SDL_DestroyWindow(win);
    log_sdl_error("SDL_CreateRenderer");
    SDL_Quit();
    destroy_cpu(cpu);
    return 1;
  }
  // how can I draw a white rectangle with sdl?
  // in the mean time I will just render my image as a "pixel"
  SDL_Texture *tex = load_texture(ren, "./data/hello.bmp");
  if (tex == NULL) {
    log_sdl_error("load_texture");
    cleanup(win, ren, NULL);
    SDL_Quit();
    destroy_cpu(cpu);
    return 1;
  }
  SDL_Delay(1000);

  /* Run Emulation Loop */
  for (;;) {
    address addr_nxt_instr = cpu->pc;
    uint8_t msb = load(cpu->mem, addr_nxt_instr);
    uint8_t lsb = load(cpu->mem, addr_nxt_instr + 1);
    opcode op_code = msb << 8 | lsb;
    printf("** Opcode - 0x%04x **\n", op_code);
    print_cpu(cpu);
    int cycles = execute_op_code(cpu, scr, keys, op_code);
    if (cycles == 0) {
      printf("Found unimplemented opcode - 0x%04x\n", op_code);
      break;
    }

    int quit = update_keyboard(keys);
    if (quit < 0) {
      break;
    }

    draw_screen(ren, tex, scr);

    // Update timers
    if (cpu->delay_timer > 0) {
      cpu->delay_timer--;
    }
    if (cpu->sound_timer > 0) {
      if(cpu->sound_timer == 1) {
        cpu->sound_timer--;
      }
    }

    SDL_Delay(10);
  }

  cleanup(win, ren, tex);
	SDL_Quit();
  destroy_cpu(cpu);

  printf("Quitting emulator.\n");
  return 0;
}
