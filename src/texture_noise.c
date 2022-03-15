#include <math.h>

#include "muon_common.h"
#include "noise_perlin.h"
#include "texture_base.h"
#include "texture_noise.h"

typedef struct {
  const Noise_Perlin *p_noise_perlin;
  double scaling_factor;
} Texture_Noise;

static C3
texture__noise__value(const Texture_Noise *p_texture_noise, double u, double v, P3 position) {
  C3 value;

  // TODO - Remove hardcoded switch, and expose a build parameter to configure.

  switch (2) {
    case 0: {
      // NOTE - No turbulence.
      value = v3__mul(
          (C3){1.0, 1.0, 1.0},
          0.5 * (1.0 + noise__perlin__value(p_texture_noise->p_noise_perlin,
                                            v3__mul(position, p_texture_noise->scaling_factor))));
      break;
    }

    case 1: {
      // NOTE - Direct turbulence (e.g. netting).
      value =
          v3__mul((C3){1.0, 1.0, 1.0},
                  noise__perlin__turbulence(p_texture_noise->p_noise_perlin,
                                            v3__mul(position, p_texture_noise->scaling_factor), 7));
      break;
    }

    case 2: {
      // NOTE - Indirect turbulence (e.g. marble).
      value = v3__mul((C3){1.0, 1.0, 1.0},
                      0.5 * (1.0 + sin(p_texture_noise->scaling_factor * position.z +
                                       10.0 * noise__perlin__turbulence(
                                                  p_texture_noise->p_noise_perlin, position, 7))));
      break;
    }
  }

  return value;
}

static const TextureInterface *p_noise_texture_interface =
    &(TextureInterface){.fp_value = (C3(*)(const void *, double, double, P3))texture__noise__value};

const Texture *texture__noise__build(MemoryArena *p_memory_arena, double scaling_factor) {
  Texture_Noise *p_instance =
      (Texture_Noise *)memory_arena__allocate(p_memory_arena, sizeof(Texture_Noise));

  p_instance->p_noise_perlin = noise__perlin__build(p_memory_arena);
  p_instance->scaling_factor = scaling_factor;

  return texture__build(p_memory_arena, p_instance, p_noise_texture_interface);
}
