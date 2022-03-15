#ifndef RAY_H
#define RAY_H

typedef struct Ray_s Ray;

#include "vector.h"

struct Ray_s {
  P3 position;
  V3 direction;
  double time;
};

P3 ray__at(const Ray *p_ray, double t);

#endif