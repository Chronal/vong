#ifndef STATE_H
#define STATE_H

#include "vec2d.h"
#include <SDL3/SDL.h>

#define PADDLE_W (0.1)
#define PADDLE_H (0.3)
#define PADDLE_SPEED (0.1)

#define LEFT_PADDLE_INIT_POS {PADDLE_W, 0.5 - PADDLE_H / 2.0};
#define RIGHT_PADDLE_INIT_POS {1.0 - 2 * PADDLE_W, 0.5 - PADDLE_H / 2.0};

#define BALL_W (0.05)
#define BALL_H (0.05)
#define BALL_INIT_SPEED (0.2)
#define BALL_INIT_POS {0.5 - BALL_W / 2, 0.5 - BALL_W / 2}

typedef struct ball ball;
struct ball {
	vec2d pos;
	vec2d velocity;
	double radius;
};

typedef struct paddle paddle;
struct paddle {
	vec2d pos;
	double speed;
};

typedef struct game_state game_state;
struct game_state {
	Uint64 last_update;
	ball b;
	paddle left_paddle;
	paddle right_paddle;
};

typedef struct {
	SDL_Window* window;
	SDL_Renderer* renderer;
	game_state game;
} app_state;

void vong_init(game_state* state);
bool vong_check_win(game_state* state, int* winner_id);
void vong_reset(game_state* state);
void vong_step(game_state* state);

#endif /* GAME_STATE_H */