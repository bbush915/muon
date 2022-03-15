#include "texture_base.h"
#include "muon_common.h"

const Texture *texture__build(MemoryArena *p_memory_arena,
                              const void *p_instance,
                              const TextureInterface *p_interface) {
  Texture *p_texture = (Texture *)memory_arena__allocate(p_memory_arena, sizeof(Texture));

  p_texture->p_instance = p_instance;
  p_texture->p_interface = p_interface;

  return p_texture;
}

C3 texture__value(const Texture *p_texture, double u, double v, P3 position) {
  return (p_texture->p_interface->fp_value)(p_texture->p_instance, u, v, position);
}