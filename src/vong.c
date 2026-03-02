#include "vong.h"
#include "state.h"

#include <SDL3/SDL.h>
#include <SDL3/SDL_gamepad.h>
#include <SDL3/SDL_log.h>
#include <SDL3/SDL_rect.h>
#include <SDL3/SDL_render.h>
#include <SDL3/SDL_video.h>
#define SDL_MAIN_USE_CALLBACKS
#include <SDL3/SDL_main.h>

SDL_AppResult handle_keypress(app_state* state, SDL_Event* event);

SDL_AppResult SDL_AppInit(void** appstate, int argc,
                          [[maybe_unused]] char* argv[argc + 1]) {

	if (!SDL_Init(SDL_INIT_VIDEO)) {
		SDL_LogError(SDL_LOG_CATEGORY_VIDEO, "%s", SDL_GetError());
		return SDL_APP_FAILURE;
	}

	app_state* as = SDL_calloc(1, sizeof(app_state));
	if (!as) {
		SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "%s", SDL_GetError());
		return SDL_APP_FAILURE;
	}

	*appstate = as;

	SDL_DisplayID primary_display = SDL_GetPrimaryDisplay();
	if (!primary_display) {
		SDL_LogError(SDL_LOG_CATEGORY_VIDEO, "%s", SDL_GetError());
		return SDL_APP_FAILURE;
	}

	const SDL_DisplayMode* display_mode =
		SDL_GetCurrentDisplayMode(primary_display);

	if (!SDL_CreateWindowAndRenderer("Vong", display_mode->w, display_mode->h,
	                                 0, &as->window, &as->renderer)) {
		SDL_LogError(SDL_LOG_CATEGORY_RENDER, "%s", SDL_GetError());
		return SDL_APP_FAILURE;
	}

	if (!SDL_SetRenderLogicalPresentation(as->renderer, LOGICAL_WIDTH,
	                                      LOGICAL_HEIGHT,
	                                      SDL_LOGICAL_PRESENTATION_LETTERBOX)) {
		SDL_LogError(SDL_LOG_CATEGORY_RENDER,
		             "Failed to set up logical presentation");
	}

	SDL_FRect screen_bounds;
	if (!SDL_GetRenderLogicalPresentationRect(as->renderer, &screen_bounds)) {
		SDL_LogError(SDL_LOG_CATEGORY_RENDER,
		             "Failed to get logical presentation rectangle");
	}

	vong_init(&(as->game), screen_bounds);

	SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION,
	            "Finished initialising application");

	return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppIterate(void* appstate) {
	app_state* state = appstate;

	SDL_Renderer* renderer = state->renderer;

	Uint64 current_tick = SDL_GetTicks();

	Uint64 ticks_elapsed = current_tick - state->game.last_update;

	if (ticks_elapsed > 10) {
		SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, SDL_ALPHA_OPAQUE);
		SDL_RenderClear(renderer);

		SDL_SetRenderDrawColor(renderer, 0x0, 0x0, 0x0, SDL_ALPHA_OPAQUE);

		const SDL_FRect screen_bounds = state->game.bounds;
		SDL_RenderFillRect(renderer, &screen_bounds);

		state->game.last_update = current_tick;

		SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, SDL_ALPHA_OPAQUE);

		ball* b = &state->game.b;
		const SDL_FRect ball = {
			.w = b->radius, .h = b->radius, .x = b->pos.x, .y = b->pos.y};

		if (!SDL_RenderFillRect(renderer, &ball)) {
			SDL_LogError(SDL_LOG_CATEGORY_RENDER, "%s", SDL_GetError());
		}

		vong_step(&state->game, ticks_elapsed);
		SDL_Log("Ball position x: %g y: %g", state->game.b.pos.x,
		        state->game.b.pos.y);

		/* SDL_Log("Ball velocity x: %g y: %g", state->game.b.velocity.x, */
		/* state->game.b.velocity.y); */

		SDL_RenderPresent(renderer);
	}

	return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppEvent(void* appstate, SDL_Event* event) {
	switch (event->type) {
	case SDL_EVENT_WINDOW_CLOSE_REQUESTED:
	case SDL_EVENT_QUIT:
		return SDL_APP_SUCCESS;
	case SDL_EVENT_KEY_DOWN:
		SDL_Log("Scancode %s DOWN", SDL_GetScancodeName(event->key.scancode));
		return handle_keypress((app_state*)appstate, event);
	case SDL_EVENT_KEY_UP:
		SDL_Log("Scancode %s UP", SDL_GetScancodeName(event->key.scancode));
	default:
		return SDL_APP_CONTINUE;
	}
}

void SDL_AppQuit(void* appstate, SDL_AppResult result) {
	app_state* as = appstate;

	if (as) {
		SDL_DestroyRenderer(as->renderer);
		SDL_DestroyWindow(as->window);
		SDL_free(as);
	}
}

SDL_AppResult handle_keypress(app_state* state, SDL_Event* event) {
	switch (event->key.scancode) {
	case SDL_SCANCODE_ESCAPE:
	case SDL_SCANCODE_Q:
		return SDL_APP_SUCCESS;
	case SDL_SCANCODE_W:
	case SDL_SCANCODE_S:

	case SDL_SCANCODE_UP:
	case SDL_SCANCODE_DOWN:
	default:
		return SDL_APP_CONTINUE;
	}
}

SDL_AppResult handle_keyrelease(app_state* state, SDL_Event* event) {}