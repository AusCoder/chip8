#include "sdl_io.h"

const int SDL_SCREEN_WIDTH  = 640;
const int SDL_SCREEN_HEIGHT = 480;

void log_sdl_error(const char *msg) {
  printf("%s error: %s\n", msg, SDL_GetError());
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
  const int SDL_PIXEL_WIDTH = SDL_SCREEN_WIDTH / SCREEN_WIDTH;
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
