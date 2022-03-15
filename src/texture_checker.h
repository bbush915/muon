#ifndef TEXTURE_CHECKER_H
#define TEXTURE_CHECKER_H

#include "muon_common.h"
#include "texture_base.h"

const Texture *texture__checker__build(MemoryArena *p_memory_arena,
                                       const Texture *p_texture_odd,
                                       const Texture *p_texture_even);

#endif