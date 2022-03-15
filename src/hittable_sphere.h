#ifndef HITTABLE_SPHERE_H
#define HITTABLE_SPHERE_H

#include "hittable_base.h"
#include "material_base.h"
#include "muon_common.h"

const Hittable *hittable__sphere__build(MemoryArena *p_memory_arena,
                                        P3 position,
                                        double radius,
                                        const Material *p_material);

const Hittable *hittable__moving_sphere__build(MemoryArena *p_memory_arena,
                                               P3 position_start,
                                               P3 position_finish,
                                               double time_start,
                                               double time_finish,
                                               double radius,
                                               const Material *p_material);

#endif