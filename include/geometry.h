#ifndef GEOMETRY_H
#define GEOMETRY_H

#include "vec2d.h"
#include <SDL3/SDL.h>

typedef struct circle circle;
struct circle {
	vec2d centre;
	double radius;
};

typedef struct rect rect;
struct rect {
	vec2d corner;
	double width;
	double height;
};
bool circle_rect_intersection(circle c, rect r);

#endif /* GEOMETRY_H */