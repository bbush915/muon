#ifndef MEMORY_ARENA_H
#define MEMORY_ARENA_H

typedef struct MemoryArena_s MemoryArena;

#include "stdlib.h"

struct MemoryArena_s {
  void *p_buffer;
  size_t buffer_length;
  size_t offset;
};

#define KILOBYTES(x) ((size_t)(x) << 10)
#define MEGABYTES(x) ((size_t)(x) << 20)

MemoryArena *memory_arena__build(size_t size);

void memory_arena__destroy(MemoryArena *p_memory_arena);

void *memory_arena__allocate(MemoryArena *p_memory_arena, size_t size);

#endif