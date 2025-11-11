#include "vong.h"

#include <SDL3/SDL.h>
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

	int mode_count;
	SDL_DisplayMode** display_modes =
		SDL_GetFullscreenDisplayModes(primary_display, &mode_count);

	SDL_DisplayMode* display_mode = display_modes[0];

	if (!SDL_CreateWindowAndRenderer("Vong", display_mode->w, display_mode->h,
	                                 SDL_WINDOW_FULLSCREEN |
	                                     SDL_WINDOW_MAXIMIZED,
	                                 &as->window, &as->renderer)) {

		SDL_LogError(SDL_LOG_CATEGORY_RENDER, "%s", SDL_GetError());
		return SDL_APP_FAILURE;
	}

	SDL_free(display_modes);
	vong_init(&(as->game));

	SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION,
	            "Finished initialising application");

	return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppIterate(void* appstate) {
	app_state* state = appstate;

	SDL_Renderer* renderer = state->renderer;

	Uint64 current_tick = SDL_GetTicks();

	Uint64 ticks_elapsed = current_tick - state->game.last_update;

	if (ticks_elapsed >= 200) {
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
		SDL_RenderClear(renderer);

		state->game.last_update = current_tick;

		int out_width;
		int out_height;
		SDL_SetRenderLogicalPresentation(
			renderer, 1000, 1000, SDL_LOGICAL_PRESENTATION_INTEGER_SCALE);

		SDL_GetRenderOutputSize(state->renderer, &out_width, &out_height);

		double ratio = (double)out_width / out_height;
		SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, SDL_ALPHA_OPAQUE);
		SDL_SetRenderScale(renderer, 1000, 1000);

		ball* b = &state->game.b;
		const SDL_FRect ball = {
			.w = b->radius, .h = b->radius, .x = b->pos.x, .y = b->pos.y};

		if (!SDL_RenderFillRect(renderer, &ball)) {
			SDL_LogError(SDL_LOG_CATEGORY_RENDER, "%s", SDL_GetError());
		}

		ball_step(b, ticks_elapsed);

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
		SDL_Log("Scacode %s DOWN", SDL_GetScancodeName(event->key.scancode));
		return handle_keypress((app_state*)appstate, event);
	case SDL_EVENT_KEY_UP:
		SDL_Log("Scacode %s UP", SDL_GetScancodeName(event->key.scancode));
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
SDL_AppResult handle_keyrelease(app_state* state, SDL_Event* event) {

};