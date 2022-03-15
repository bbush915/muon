#ifndef MUON_UTILS_H
#define MUON_UTILS_H

const double PI;

int utils__random_int_in_range(int minimum, int maximum);

double utils__random_double();

double utils__random_double_in_range(double minimum, double maximum);

double utils__clamp(double value, double minimum, double maximum);

double utils__degrees_to_radians(double degrees);

#endif