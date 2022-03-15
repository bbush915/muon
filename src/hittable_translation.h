#ifndef HITTABLE_TRANSLATION_H
#define HITTABLE_TRANSLATION_H

#include "hittable_base.h"
#include "muon_common.h"

const Hittable *
hittable__translation__build(MemoryArena *p_memory_arena, const Hittable *p_hittable, V3 offset);

#endif