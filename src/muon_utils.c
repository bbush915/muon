#include <stdlib.h>

#include "muon_utils.h"

const double PI = 3.1415926535897932385;

int utils__random_int_in_range(int minimum, int maximum) {
  return (int)utils__random_double_in_range(minimum, maximum + 1);
}

double utils__random_double() {
  return (double)rand() / (RAND_MAX + 1.0);
}

double utils__random_double_in_range(double minimum, double maximum) {
  return minimum + (maximum - minimum) * utils__random_double();
}

double utils__clamp(double value, double minimum, double maximum) {
  return value < minimum ? minimum : (value > maximum ? maximum : value);
}

double utils__degrees_to_radians(double degrees) {
  return degrees * PI / 180.0;
}