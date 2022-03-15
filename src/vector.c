#include <math.h>
#include <stdbool.h>

#include "muon_utils.h"
#include "vector.h"

V3 v3__neg(V3 a) {
  return v3__mul(a, -1.0);
}

V3 v3__add(V3 a, V3 b) {
  return (V3){.x = a.x + b.x, .y = a.y + b.y, .z = a.z + b.z};
}

V3 v3__sub(V3 a, V3 b) {
  return (V3){.x = a.x - b.x, .y = a.y - b.y, .z = a.z - b.z};
}

V3 v3__mul(V3 a, double c) {
  return (V3){.x = a.x * c, .y = a.y * c, .z = a.z * c};
}

V3 v3__div(V3 a, double c) {
  return (V3){.x = a.x / c, .y = a.y / c, .z = a.z / c};
}

double v3__len(V3 a) {
  return sqrt(v3__len2(a));
}

double v3__len2(V3 a) {
  return v3__dot(a, a);
}

bool v3__is_near_zero(V3 a) {
  const double tolerance = 1e-8;
  return (fabs(a.x) < tolerance) && (fabs(a.y) < tolerance) && (fabs(a.z) < tolerance);
}

double v3__dot(V3 a, V3 b) {
  return a.x * b.x + a.y * b.y + a.z * b.z;
}

V3 v3__hadamard(V3 a, V3 b) {
  return (V3){.x = a.x * b.x, .y = a.y * b.y, .z = a.z * b.z};
}

V3 v3__cross(V3 a, V3 b) {
  return (V3){
      .x = a.y * b.z - a.z * b.y,
      .y = a.z * b.x - a.x * b.z,
      .z = a.x * b.y - a.y * b.x,
  };
}

V3 v3__unit(V3 a) {
  return v3__div(a, v3__len(a));
}

V3 v3__reflect(V3 a, V3 n) {
  return v3__sub(a, v3__mul(n, 2.0 * v3__dot(a, n)));
}

V3 v3__refract(V3 a, V3 n, double refrative_index_ratio) {
  const double cosine_theta = fmin(v3__dot(v3__neg(a), n), 1.0);

  const V3 v_perpendicular = v3__mul((v3__add(a, v3__mul(n, cosine_theta))), refrative_index_ratio);
  const V3 v_parallel = v3__mul(n, -sqrt(fabs(1.0 - v3__len2(v_perpendicular))));

  return v3__add(v_perpendicular, v_parallel);
}

V3 v3__random() {
  return (V3){
      .x = utils__random_double(), .y = utils__random_double(), .z = utils__random_double()};
}

V3 v3__random_in_range(double minimum, double maximum) {
  return (V3){.x = utils__random_double_in_range(minimum, maximum),
              .y = utils__random_double_in_range(minimum, maximum),
              .z = utils__random_double_in_range(minimum, maximum)};
}

V3 v3__random_in_disk(double radius) {
  V3 result;

  do {
    result = (V3){.x = utils__random_double_in_range(-radius, radius),
                  .y = utils__random_double_in_range(-radius, radius),
                  .z = 0.0};
  } while (v3__len2(result) > 1.0);

  return result;
}

V3 v3__random_in_sphere(double radius) {
  V3 result;

  do {
    result = v3__random_in_range(-radius, radius);
  } while (v3__len2(result) > 1.0);

  return result;
}