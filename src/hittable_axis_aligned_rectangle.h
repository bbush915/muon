#ifndef HITTABLE_AXIS_ALIGNED_RECTANGLE_H
#define HITTABLE_AXIS_ALIGNED_RECTANGLE_H

#include "hittable_base.h"
#include "material_base.h"
#include "muon_common.h"

// NOTE - XY Rectangle

const Hittable *hittable__xy_rectangle__build(MemoryArena *p_memory_arena,
                                              double x0,
                                              double x1,
                                              double y0,
                                              double y1,
                                              double k,
                                              const Material *p_material);

// NOTE - XZ Rectangle

const Hittable *hittable__xz_rectangle__build(MemoryArena *p_memory_arena,
                                              double x0,
                                              double x1,
                                              double z0,
                                              double z1,
                                              double k,
                                              const Material *p_material);

// NOTE - YZ Rectangle

const Hittable *hittable__yz_rectangle__build(MemoryArena *p_memory_arena,
                                              double y0,
                                              double y1,
                                              double z0,
                                              double z1,
                                              double k,
                                              const Material *p_material);

#endif