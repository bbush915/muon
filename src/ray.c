#include "ray.h"
#include "vector.h"

P3 ray__at(const Ray *p_ray, double t) {
  return v3__add(p_ray->position, v3__mul(p_ray->direction, t));
}