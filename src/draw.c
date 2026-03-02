#include "draw.h"
#include "state.h"
#include <SDL3/SDL.h>

void draw_vong(app_state* as) {
	SDL_Renderer* renderer = as->renderer;
	game_state* gs = &as->game;

	SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, SDL_ALPHA_OPAQUE);
	SDL_RenderClear(renderer);

	SDL_SetRenderDrawColor(renderer, 0x0, 0x0, 0x0, SDL_ALPHA_OPAQUE);

	const SDL_FRect screen_bounds = gs->bounds;
	SDL_RenderFillRect(renderer, &screen_bounds);
	SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, SDL_ALPHA_OPAQUE);

	ball b = gs->b;
	const SDL_FRect ball = {
		.w = b.radius, .h = b.radius, .x = b.pos.x, .y = b.pos.y};

	if (!SDL_RenderFillRect(renderer, &ball)) {
		SDL_LogError(SDL_LOG_CATEGORY_RENDER, "%s", SDL_GetError());
	}

	SDL_RenderPresent(renderer);
}