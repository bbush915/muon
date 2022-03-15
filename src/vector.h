#ifndef VECTOR_H
#define VECTOR_H

#include <stdbool.h>

typedef union {
  struct {
    double x;
    double y;
    double z;
  };

  double components[3];
} V3;

typedef V3 C3;
typedef V3 P3;

V3 v3__neg(V3 a);

V3 v3__add(V3 a, V3 b);

V3 v3__sub(V3 a, V3 b);

V3 v3__mul(V3 a, double c);

V3 v3__div(V3 a, double c);

double v3__len(V3 a);

double v3__len2(V3 a);

bool v3__is_near_zero(V3 a);

double v3__dot(V3 a, V3 b);

V3 v3__hadamard(V3 a, V3 b);

V3 v3__cross(V3 a, V3 b);

V3 v3__unit(V3 a);

V3 v3__reflect(V3 a, V3 n);

V3 v3__refract(V3 a, V3 n, double refrative_index_ratio);

V3 v3__random();

V3 v3__random_in_range(double minimum, double maximuum);

V3 v3__random_in_disk(double radius);

V3 v3__random_in_sphere(double radius);

#endif