#ifndef HITTABLE_BOUNDING_VOLUME_HIERARCHY_H
#define HITTABLE_BOUNDING_VOLUME_HIERARCHY_H

#include "hittable_base.h"
#include "muon_common.h"

const Hittable *hittable__bounding_volume_hierarchy__build(MemoryArena *p_memory_arena,
                                                           const Hittable **pp_hittables,
                                                           int hittable_count,
                                                           int index_start,
                                                           int index_finish,
                                                           double t0,
                                                           double t1);

#endif