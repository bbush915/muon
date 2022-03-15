#ifndef HITTABLE_ROTATION_H
#define HITTABLE_ROTATION_H

#include "hittable_base.h"
#include "muon_common.h"

// NOTE - Y-axis rotation.

const Hittable *hittable__y_rotation__build(MemoryArena *p_memory_arena,
                                            const Hittable *p_hittable,
                                            double degrees);

#endif