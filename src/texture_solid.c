#include "texture_solid.h"
#include "muon_common.h"
#include "texture_base.h"

typedef struct {
  C3 color;
} Texture_Solid;

static C3
texture__solid__value(const Texture_Solid *p_texture_solid, double u, double v, P3 position) {
  return p_texture_solid->color;
}

static const TextureInterface *p_solid_texture_interface =
    &(TextureInterface){.fp_value = (C3(*)(const void *, double, double, P3))texture__solid__value};

const Texture *texture__solid__build(MemoryArena *p_memory_arena, C3 color) {
  Texture_Solid *p_instance =
      (Texture_Solid *)memory_arena__allocate(p_memory_arena, sizeof(Texture_Solid));

  p_instance->color = color;

  return texture__build(p_memory_arena, p_instance, p_solid_texture_interface);
}
