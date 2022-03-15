#ifndef HITTABLE_BOX_H
#define HITTABLE_BOX_H

#include "hittable_base.h"
#include "material_base.h"
#include "muon_common.h"

const Hittable *hittable__box__build(MemoryArena *p_memory_arena,
                                     P3 position_minimum,
                                     P3 position_maximum,
                                     const Material *p_material);

#endif