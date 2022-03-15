#include <math.h>
#include <stdbool.h>

#include "axis_aligned_bounding_box.h"
#include "vector.h"

bool aabb__hit(const AABB *p_aabb, const Ray *p_ray, double t_min, double t_max) {
  for (int i = 0; i < 3; i++) {
    const double inv_d = 1.0 / p_ray->direction.components[i];

    double t0 = (p_aabb->position_minimum.components[i] - p_ray->position.components[i]) * inv_d;
    double t1 = (p_aabb->position_maximum.components[i] - p_ray->position.components[i]) * inv_d;

    if (inv_d < 0.0) {
      double tmp = t0;
      t0 = t1;
      t1 = tmp;
    }

    t_min = t0 > t_min ? t0 : t_min;
    t_max = t1 < t_max ? t1 : t_max;

    if (t_max <= t_min) {
      return false;
    }
  }

  return true;
}

AABB aabb__surrounding_bounding_box(AABB a, AABB b) {
  const P3 position_minimum = (P3){fmin(a.position_minimum.x, b.position_minimum.x),
                                   fmin(a.position_minimum.y, b.position_minimum.y),
                                   fmin(a.position_minimum.z, b.position_minimum.z)};

  const P3 position_maximum = (P3){fmax(a.position_maximum.x, b.position_maximum.x),
                                   fmax(a.position_maximum.y, b.position_maximum.y),
                                   fmax(a.position_maximum.z, b.position_maximum.z)};

  return (AABB){.position_minimum = position_minimum, .position_maximum = position_maximum};
}