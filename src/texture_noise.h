#ifndef TEXTURE_NOISE_H
#define TEXTURE_NOISE_H

#include "muon_common.h"
#include "texture_base.h"

const Texture *texture__noise__build(MemoryArena *p_memory_arena, double scaling_factor);

#endif