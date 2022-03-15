#ifndef AXIS_ALIGNED_BOUNDING_BOX_H
#define AXIS_ALIGNED_BOUNDING_BOX_H

#include <stdbool.h>

typedef struct AABB_s AABB;

#include "ray.h"
#include "vector.h"

struct AABB_s {
  P3 position_minimum;
  P3 position_maximum;
};

bool aabb__hit(const AABB *p_aabb, const Ray *p_ray, double t_min, double t_max);

AABB aabb__surrounding_bounding_box(AABB a, AABB b);

#endif