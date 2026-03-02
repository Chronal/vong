#include "vec2d.h"

#include <SDL3/SDL.h>

double vec2d_dot(vec2d a, vec2d b) { return a.x * b.x + a.y * b.y; }

double vec2d_len(vec2d v) { return SDL_sqrt(vec2d_dot(v, v)); }

vec2d vec2d_flip_x(vec2d* v) {
	v->x *= -1.0;
	return *v;
}

vec2d vec2d_flip_y(vec2d* v) {
	v->y *= -1.0;
	return *v;
}

vec2d vec2d_add(vec2d a, vec2d b) {
	vec2d sum = {.x = a.x + b.x, .y = a.y + b.y};
	return sum;
}

vec2d vec2d_inv(vec2d v) {
	vec2d flipped = vec2d_scale(&v, -1.0);
	return flipped;
}

vec2d vec2d_scale(vec2d* v, double scalar) {
	v->x *= scalar;
	v->y *= scalar;

	return *v;
}

vec2d vec2d_rand_dir() {
	float angle = 2 * SDL_PI_F * SDL_randf();
	vec2d v = {.x = SDL_cosf(angle), .y = SDL_sinf(angle)};
	return v;
}

void vec2d_set(vec2d* v, vec2d c) {
	v->x = c.x;
	v->y = c.y;
}