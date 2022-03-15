#ifndef NOISE_PERLIN_H
#define NOISE_PERLIN_H

typedef struct Noise_Perlin_s Noise_Perlin;

#include "muon_common.h"
#include "vector.h"

struct Noise_Perlin_s {
  V3 random_unit_vectors[255];

  int permutation_x[255];
  int permutation_y[255];
  int permutation_z[255];
};

const Noise_Perlin *noise__perlin__build(MemoryArena *p_memory_arena);

double noise__perlin__value(const Noise_Perlin *p_noise_perlin, P3 position);

double noise__perlin__turbulence(const Noise_Perlin *p_noise_perlin, P3 position, int depth);

#endif