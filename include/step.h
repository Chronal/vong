#ifndef GAME_H
#define GAME_H

#include "state.h"

void paddle_init(paddle* p, vec2d pos);
void paddle_step(paddle* p, Uint64 ticks);

void ball_init(ball* b);
void ball_step(ball* b, Uint64 ticks);

#endif