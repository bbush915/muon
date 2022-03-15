#include <math.h>

#include "muon_common.h"
#include "texture_base.h"
#include "texture_checker.h"

typedef struct {
  const Texture *p_texture_odd;
  const Texture *p_texture_even;
} Texture_Checker;

static C3
texture__checker__value(const Texture_Checker *p_texture_checker, double u, double v, P3 position) {
  const double value = sin(10.0 * position.x) * sin(10.0 * position.y) * sin(10.0 * position.z);

  return (value < 0.0) ? texture__value(p_texture_checker->p_texture_odd, u, v, position)
                       : texture__value(p_texture_checker->p_texture_even, u, v, position);
}

static const TextureInterface *p_checker_texture_interface = &(TextureInterface){
    .fp_value = (C3(*)(const void *, double, double, P3))texture__checker__value};

const Texture *texture__checker__build(MemoryArena *p_memory_arena,
                                       const Texture *p_texture_odd,
                                       const Texture *p_texture_even) {
  Texture_Checker *p_instance =
      (Texture_Checker *)memory_arena__allocate(p_memory_arena, sizeof(Texture_Checker));

  p_instance->p_texture_odd = p_texture_odd;
  p_instance->p_texture_even = p_texture_even;

  return texture__build(p_memory_arena, p_instance, p_checker_texture_interface);
}
