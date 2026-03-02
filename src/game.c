#include "state.h"
#include "vec2d.h"
#include <SDL3/SDL.h>
#include <SDL3/SDL_stdinc.h>

static void ball_init(ball* b, SDL_FRect bounds);

void paddle_init(paddle* p, vec2d pos) {
	p->speed = PADDLE_SPEED;
	vec2d_set(&p->pos, pos);
}

void vong_init(game_state* game, SDL_FRect bounds) {
	game->bounds = bounds;
	// One paddle width gap between goal zone
	// and the paddles
	vec2d left_pos = LEFT_PADDLE_INIT_POS;
	vec2d right_pos = RIGHT_PADDLE_INIT_POS;

	game->last_update = SDL_GetTicks();
	/* paddle_init(&game->left_paddle, left_pos); */
	/* paddle_init(&game->left_paddle, right_pos); */

	ball_init(&game->b, game->bounds);
}

void paddle_step(paddle* p, Uint64 ticks) {}

void ball_init(ball* b, SDL_FRect bounds) {
	b->radius = BALL_W;

	vec2d init_pos = {bounds.w / 2 - BALL_W / 2, bounds.h / 2 - BALL_H / 2};
	vec2d_set(&b->pos, init_pos);

	vec2d_set(&b->velocity, vec2d_rand_dir());
	vec2d_scale(&b->velocity, BALL_INIT_SPEED);
}

void ball_step(ball* b, Uint64 tick_diff, SDL_FRect bounds) {
	double seconds = tick_diff / 1e3;

	vec2d next_pos = b->pos;
	next_pos.x += b->velocity.x * seconds;
	next_pos.y += b->velocity.y * seconds;

	if (next_pos.y < 0.0 || next_pos.y > (bounds.h - BALL_H)) {
		vec2d_flip_y(&b->velocity);
	}

	/* next_pos.x = SDL_clamp(next_pos.x, 0.0, bounds.w - BALL_W); */
	next_pos.y = SDL_clamp(next_pos.y, 0.0, bounds.h - BALL_H);

	vec2d_set(&b->pos, next_pos);
}

void vong_step(game_state* state, Uint64 tick_delta) {
	ball_step(&state->b, tick_delta, state->bounds);
}

void vong_reset(game_state* state) {}

bool vong_check_win(game_state* state, int* winner_id) {
	if (state->b.pos.x > 1.0) {
		*winner_id = 0;
		return true;
	} else if (state->b.pos.y < 0.0) {
		*winner_id = 1;
		return true;
	} else {
		return false;
	}
}