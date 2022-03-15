#include <math.h>

#include "muon_common.h"
#include "noise_perlin.h"

static void swap(int *values, int i, int j) {
  const int temp = values[i];
  values[i] = values[j];
  values[j] = temp;
}

static void shuffle(int *values, int value_count) {
  for (int i = value_count - 1; i > 0; i--) {
    const int target = utils__random_int_in_range(0, i);
    swap(values, i, target);
  }
}

static double trilinear_interpolation(V3 colors[2][2][2], double u, double v, double w) {
  // NOTE - Hermitian smoothing.

  const double uu = u * u * (3 - 2 * u);
  const double vv = v * v * (3 - 2 * v);
  const double ww = w * w * (3 - 2 * w);

  // NOTE - Use a dot product to move the minimum and maximum off the lattice.

  double accumulator = 0.0;

  for (int i = 0; i < 2; i++) {
    for (int j = 0; j < 2; j++) {
      for (int k = 0; k < 2; k++) {
        const V3 weights = (V3){.x = u - i, .y = v - j, .z = w - k};

        accumulator += (i * uu + (1 - i) * (1 - uu)) * (j * vv + (1 - j) * (1 - vv)) *
                       (k * ww + (1 - k) * (1 - ww)) * v3__dot(colors[i][j][k], weights);
      }
    }
  }

  return accumulator;
}

const Noise_Perlin *noise__perlin__build(MemoryArena *p_memory_arena) {
  Noise_Perlin *p_noise_perlin =
      (Noise_Perlin *)memory_arena__allocate(p_memory_arena, sizeof(Noise_Perlin));

  for (int i = 0; i < 255; i++) {
    p_noise_perlin->random_unit_vectors[i] = v3__unit(v3__random_in_range(-1.0, 1.0));

    p_noise_perlin->permutation_x[i] = i;
    p_noise_perlin->permutation_y[i] = i;
    p_noise_perlin->permutation_z[i] = i;
  }

  shuffle(p_noise_perlin->permutation_x, 255);
  shuffle(p_noise_perlin->permutation_y, 255);
  shuffle(p_noise_perlin->permutation_z, 255);

  return p_noise_perlin;
}

double noise__perlin__value(const Noise_Perlin *p_noise_perlin, P3 position) {
  const double u = position.x - floor(position.x);
  const double v = position.y - floor(position.y);
  const double w = position.z - floor(position.z);

  const int i = (int)(floor(position.x)) & 255;
  const int j = (int)(floor(position.y)) & 255;
  const int k = (int)(floor(position.z)) & 255;

  V3 colors[2][2][2];

  for (int di = 0; di < 2; di++) {
    for (int dj = 0; dj < 2; dj++) {
      for (int dk = 0; dk < 2; dk++) {
        colors[di][dj][dk] =
            p_noise_perlin->random_unit_vectors[p_noise_perlin->permutation_x[(i + di) & 255] ^
                                                p_noise_perlin->permutation_y[(j + dj) & 255] ^
                                                p_noise_perlin->permutation_z[(k + dk) & 255]];
      }
    }
  }

  return trilinear_interpolation(colors, u, v, w);
}

double noise__perlin__turbulence(const Noise_Perlin *p_noise_perlin, P3 position, int depth) {
  double accumulator = 0.0;
  double weight = 1.0;

  for (int i = 0; i < depth; i++) {
    accumulator += weight * noise__perlin__value(p_noise_perlin, position);
    weight *= 0.5;

    position = v3__mul(position, 2.0);
  }

  return fabs(accumulator);
}