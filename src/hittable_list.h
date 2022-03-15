#ifndef HITTABLE_LIST_H
#define HITTABLE_LIST_H

#include "hittable_base.h"
#include "muon_common.h"

const Hittable *hittable__list__build(MemoryArena *p_memory_arena,
                                      const Hittable **pp_hittables,
                                      int hittable_count);

#endif