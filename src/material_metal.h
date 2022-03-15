#ifndef MATERIAL_METAL_H
#define MATERIAL_METAL_H

#include "material_base.h"
#include "muon_common.h"

const Material *
material__metal__build(MemoryArena *p_memory_arena, C3 albedo, double fuzzing_factor);

#endif