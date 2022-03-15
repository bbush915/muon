#ifndef HITTABLE_CONSTANT_MEDIUM_H
#define HITTABLE_CONSTANT_MEDIUM_H

#include "hittable_base.h"
#include "material_base.h"
#include "muon_common.h"

const Hittable *hittable__constant_medium__build(MemoryArena *p_memory_arena,
                                                 const Hittable *p_boundary,
                                                 double density,
                                                 const Material *p_material);

#endif