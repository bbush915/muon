#include <stdlib.h>

#include "memory_arena.h"

MemoryArena *memory_arena__build(size_t size) {
  MemoryArena *p_memory_arena = (MemoryArena *)malloc(sizeof(MemoryArena));

  p_memory_arena->p_buffer = (void *)malloc(size * sizeof(uint8_t));
  p_memory_arena->buffer_length = size;
  p_memory_arena->offset = 0;

  return p_memory_arena;
}

void memory_arena__destroy(MemoryArena *p_memory_arena) {
  free(p_memory_arena->p_buffer);
  free(p_memory_arena);
}

void *memory_arena__allocate(MemoryArena *p_memory_arena, size_t size) {
  if (p_memory_arena->offset + size > p_memory_arena->buffer_length) {
    return NULL;
  }

  void *p_memory = (uint8_t *)p_memory_arena->p_buffer + p_memory_arena->offset;

  p_memory_arena->offset += size;

  return p_memory;
}