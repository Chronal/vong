#include <SDL3/SDL.h>
#include <SDL3/SDL_pixels.h>
#include <SDL3/SDL_render.h>
#include <SDL3/SDL_stdinc.h>
#include <SDL3/SDL_timer.h>

#define SDL_MAIN_USE_CALLBACKS
#include <SDL3/SDL_main.h>

#include <vulkan/vulkan.h>

#define WINDOW_HEIGHT (1080)
#define WINDOW_WIDTH (1920)

typedef struct {
	int x;
	int y;
} position;

typedef struct {
	position pos;
	int radius;
	int velocity;
} ball;

typedef struct {
	position pos;
	unsigned int width;
	unsigned int speed;
} paddle;

typedef struct {
	Uint64 last_update;
	ball ball;
	paddle left_paddle;
	paddle right_paddle;
} game_state;

typedef struct {
	SDL_Window *window;
	SDL_Renderer *renderer;
	game_state state;
} app_state;

SDL_AppResult handle_keypress(app_state *state, SDL_Event *event);

SDL_AppResult SDL_AppInit(void **appstate, int argc, char **argv) {

	if (!SDL_Init(SDL_INIT_VIDEO)) {
		return SDL_APP_FAILURE;
	}

	app_state *as = SDL_calloc(1, sizeof(app_state));
	if (as == NULL) {
		return SDL_APP_FAILURE;
	}

	*appstate = as;

	if (!SDL_CreateWindowAndRenderer("Vong", WINDOW_WIDTH, WINDOW_HEIGHT, 0,
	                                 &as->window, &as->renderer)) {
		return SDL_APP_FAILURE;
	}

	Uint8 r = 255;
	Uint8 g = 0;
	Uint8 b = 0;

	SDL_SetRenderDrawColor(as->renderer, r, g, b, SDL_ALPHA_OPAQUE);
	SDL_RenderClear(as->renderer);
	as->state.last_update = SDL_GetTicks();

	return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppIterate(void *appstate) {
	app_state *state = appstate;

	SDL_Renderer *renderer = state->renderer;

	Uint64 current_tick = SDL_GetTicks();

	SDL_RenderClear(renderer);

	if (current_tick - state->state.last_update >= 1000) {
		state->state.last_update = current_tick;
		float r = SDL_randf();
		float g = SDL_randf();
		float b = SDL_randf();

		SDL_SetRenderDrawColorFloat(renderer, r, g, b, SDL_ALPHA_OPAQUE_FLOAT);
	}

	SDL_RenderPresent(renderer);
	return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppEvent(void *appstate, SDL_Event *event) {
	switch (event->type) {
	case SDL_EVENT_WINDOW_CLOSE_REQUESTED:
	case SDL_EVENT_QUIT:
		return SDL_APP_SUCCESS;
	case SDL_EVENT_KEY_DOWN:
		return handle_keypress((app_state *)appstate, event);

	default:
		return SDL_APP_CONTINUE;
	}
}

void SDL_AppQuit(void *appstate, SDL_AppResult result) {
	app_state *as = appstate;

	if (as) {
		SDL_DestroyRenderer(as->renderer);
		SDL_DestroyWindow(as->window);
		SDL_free(as);
	}
}

SDL_AppResult handle_keypress(app_state *state, SDL_Event *event) {
	switch (event->key.scancode) {
	case SDL_SCANCODE_ESCAPE:
	case SDL_SCANCODE_Q:
		return SDL_APP_SUCCESS;
	default:
		return SDL_APP_CONTINUE;
	}
}