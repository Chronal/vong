#include "draw.h"
#include "state.h"
#include <SDL3/SDL.h>

void draw_vong(app_state* as) {
	SDL_Renderer* renderer = as->renderer;
	game_state* gs = &as->game;

	SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
	SDL_RenderClear(renderer);

	int out_width;
	int out_height;

	SDL_GetRenderOutputSize(as->renderer, &out_width, &out_height);
}