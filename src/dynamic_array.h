#ifndef DYNAMIC_ARRAY_H
#define DYNAMIC_ARRAY_H

typedef struct DynamicArray_s DynamicArray;

#include "memory_arena.h"

struct DynamicArray_s {
  int item_size;
  int item_count;
  int capacity;
  void **pp_items;
  MemoryArena *p_memory_arena;
};

DynamicArray *
dynamic_array__build(MemoryArena *p_memory_arena, int item_size, int initial_capacity);

void dynamic_array__push(DynamicArray *p_dynamic_array, void *p_item);

void *dynamic_array__at(const DynamicArray *p_dynamic_array, int index);

#endif