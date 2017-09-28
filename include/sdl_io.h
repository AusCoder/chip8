
#ifndef _SDL_IO_H_
#define _SDL_IO_H_

#include "emulator.h"
#include <SDL.h>

const int SDL_SCREEN_WIDTH;
const int SDL_SCREEN_HEIGHT;

void log_sdl_error(const char *);

SDL_Texture * load_texture(SDL_Renderer *, const char *);
void render_texture(SDL_Renderer *, SDL_Texture *, int, int, int, int);
void cleanup(SDL_Window *, SDL_Renderer *, SDL_Texture *);

// draw our chip8 screen to sdl
int draw_screen(SDL_Renderer *, SDL_Texture *, Screen *scr);
// modify the Keyboard.
int update_keyboard(Keyboard *);

#endif
