#ifndef VEC2D_H
#define VEC2D_H

typedef struct vec2d vec2d;
struct vec2d {
	double x;
	double y;
};

double vec2d_dot(vec2d a, vec2d b);
double vec2d_len(vec2d v);

vec2d vec2d_rand_dir();

vec2d vec2d_flip_x(vec2d* v);
vec2d vec2d_flip_y(vec2d* v);
void vec2d_set(vec2d* v, vec2d c);

vec2d vec2d_add(vec2d a, vec2d b);
vec2d vec2d_inv(vec2d v);
vec2d vec2d_scale(vec2d* v, double scalar);

#endif /* VEC2D_H */