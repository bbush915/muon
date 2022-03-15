#ifndef TEXTURE_BASE_H
#define TEXTURE_BASE_H

typedef struct Texture_s Texture;
typedef struct TextureInterface_s TextureInterface;

#include "muon_common.h"

struct Texture_s {
  const void *p_instance;
  const TextureInterface *p_interface;
};

const Texture *texture__build(MemoryArena *p_memory_arena,
                              const void *p_instance,
                              const TextureInterface *p_interface);

struct TextureInterface_s {
  C3 (*const fp_value)(const void *p_instance, double u, double v, P3 position);
};

C3 texture__value(const Texture *p_texture, double u, double v, P3 position);

#endif