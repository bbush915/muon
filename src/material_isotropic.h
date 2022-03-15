#ifndef MATERIAL_ISOTROPIC_H
#define MATERIAL_ISOTROPIC_H

#include "material_base.h"
#include "muon_common.h"
#include "texture_base.h"

const Material *material__isotropic__build(MemoryArena *p_memory_arena, const Texture *p_texture);

#endif