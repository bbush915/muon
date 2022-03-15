#ifndef MATERIAL_DIFFUSE_LIGHT_H
#define MATERIAL_DIFFUSE_LIGHT_H

#include "material_base.h"
#include "muon_common.h"
#include "texture_base.h"

const Material *material__diffuse_light__build(MemoryArena *p_memory_arena,
                                               const Texture *p_texture);

#endif