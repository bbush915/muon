#ifndef MATERIAL_DIALECTRIC_H
#define MATERIAL_DIALECTRIC_H

#include "material_base.h"
#include "muon_common.h"

const Material *material__dialectric__build(MemoryArena *p_memory_arena, double refractive_index);

#endif