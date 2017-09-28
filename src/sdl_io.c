#include "sdl_io.h"

const int SDL_SCREEN_WIDTH  = 640;
const int SDL_SCREEN_HEIGHT = 480;

void log_sdl_error(const char *msg) {
  printf("SDL Error: %s error: %s\n", msg, SDL_GetError());
}

SDL_Texture * load_texture(SDL_Renderer *renderer, const char *bmp_path) {
  SDL_Texture *texture = NULL;
  SDL_Surface *bmp = SDL_LoadBMP(bmp_path);
  if (bmp == NULL) {
    log_sdl_error("SDL_LoadBMP");
    return texture;
  }

  texture = SDL_CreateTextureFromSurface(renderer, bmp);
  SDL_FreeSurface(bmp);
  if (texture == NULL) {
    log_sdl_error("SDL_CreateTextureFromSurface");
  }
  return texture;
}

void render_texture(SDL_Renderer *ren, SDL_Texture *tex, int x, int y, int w, int h) {
  SDL_Rect dst;
  dst.x = x;
  dst.y = y;
  dst.w = w;
  dst.h = h;
  SDL_RenderCopy(ren, tex, NULL, &dst);
}

void cleanup(SDL_Window *win, SDL_Renderer *ren, SDL_Texture *tex) {
  if (win != NULL) {
    SDL_DestroyWindow(win);
  }
  if (ren != NULL) {
    SDL_DestroyRenderer(ren);
  }
  if (tex != NULL) {
    SDL_DestroyTexture(tex);
  }
}

int draw_screen(SDL_Renderer *ren, SDL_Texture *pixel, Screen *scr) {
  // FIXME: find somewhere else for these two
  const int SDL_PIXEL_WIDTH  = SDL_SCREEN_WIDTH / SCREEN_WIDTH;
  const int SDL_PIXEL_HEIGHT = SDL_SCREEN_HEIGHT / SCREEN_HEIGHT;

  // First clear the renderer
  SDL_RenderClear(ren);
  // Now RenderCopy the pixels.
  int x, y;
  for (y=0; y < SCREEN_HEIGHT; y++) {
    for (x=0; x < SCREEN_WIDTH; x++) {
      int sdl_x = x * SDL_PIXEL_WIDTH;
      int sdl_y = y * SDL_PIXEL_HEIGHT;
      int scr_idx = y * SCREEN_WIDTH + x;
      if (scr->ar[scr_idx] & 0x1) {
        render_texture(ren, pixel, sdl_x, sdl_y, SDL_PIXEL_WIDTH, SDL_PIXEL_HEIGHT);
      } else {
        // don't render anything, maybe replace with a blank pixel?
      }
    }
  }
  // Update the screen
  SDL_RenderPresent(ren);
  return 0;
}

int translate_sdl_to_chip8_key(int keysym) {
  switch (keysym) {
  case SDLK_1:
    return CHIP8_1;
  case SDLK_2:
    return CHIP8_2;
  case SDLK_3:
    return CHIP8_3;
  case SDLK_4:
    return CHIP8_c;

  case SDLK_q:
    return CHIP8_4;
  case SDLK_w:
    return CHIP8_5;
  case SDLK_e:
    return CHIP8_6;
  case SDLK_r:
    return CHIP8_d;

  case SDLK_a:
    return CHIP8_7;
  case SDLK_s:
    return CHIP8_8;
  case SDLK_d:
    return CHIP8_9;
  case SDLK_f:
    return CHIP8_e;

  case SDLK_z:
    return CHIP8_a;
  case SDLK_x:
    return CHIP8_0;
  case SDLK_c:
    return CHIP8_b;
  case SDLK_v:
    return CHIP8_f;
  default:
    return -1;
  }
}


int update_keyboard(Keyboard *keys) {
  SDL_Event next_event;
  while(SDL_PollEvent(&next_event)) {
    if (next_event.type == SDL_KEYDOWN) {
      // set Keyboard to 0x1
      int chip8_key = translate_sdl_to_chip8_key(next_event.key.keysym.sym);
      if (chip8_key > -1) {
        keys->ar[chip8_key] = 0x1;
      } else {
        printf("Unknown key press\n");
      }
    }
    if (next_event.type == SDL_KEYUP) {
      // set Keyboard to 0x0
      int chip8_key = translate_sdl_to_chip8_key(next_event.key.keysym.sym);
      if (chip8_key > -1) {
        keys->ar[chip8_key] = 0x0;
      } else {
        printf("Unknown key press\n");
      }
    }
  }
  return 0;
}

int blocking_keyboard_read() {
  printf("Waiting for input at blocking keyboard.\n");
  SDL_Event next_event;
  int ret = SDL_WaitEvent(&next_event);
  if (ret == 0) {
    log_sdl_error("SDL_WaitEvent");
    return -1;
  }
  int chip8_key = translate_sdl_to_chip8_key(next_event.key.keysym.sym);
  return chip8_key;
}
