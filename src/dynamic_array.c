#include <stdlib.h>
#include <string.h>

#include "dynamic_array.h"
#include "muon_common.h"

static void dynamic_array__resize(DynamicArray *p_dynamic_array) {
  void **pp_items = (void **)memory_arena__allocate(p_dynamic_array->p_memory_arena,
                                                    2L * p_dynamic_array->capacity *
                                                        (size_t)p_dynamic_array->item_size);

  memcpy(pp_items, p_dynamic_array->pp_items,
         2L * p_dynamic_array->capacity * (size_t)p_dynamic_array->item_size);

  p_dynamic_array->pp_items = pp_items;

  p_dynamic_array->capacity *= 2;
}

DynamicArray *
dynamic_array__build(MemoryArena *p_memory_arena, int item_size, int initial_capacity) {
  DynamicArray *p_dynamic_array =
      (DynamicArray *)memory_arena__allocate(p_memory_arena, sizeof(DynamicArray));

  p_dynamic_array->item_size = item_size;
  p_dynamic_array->item_count = 0;
  p_dynamic_array->capacity = initial_capacity;
  p_dynamic_array->pp_items =
      (void **)memory_arena__allocate(p_memory_arena, initial_capacity * (size_t)item_size);
  p_dynamic_array->p_memory_arena = p_memory_arena;

  return p_dynamic_array;
}

void dynamic_array__push(DynamicArray *p_dynamic_array, void *p_item) {
  if (p_dynamic_array->item_count == p_dynamic_array->capacity) {
    dynamic_array__resize(p_dynamic_array);
  }

  memcpy((uint8_t *)p_dynamic_array->pp_items +
             (ptrdiff_t)(p_dynamic_array->item_count * p_dynamic_array->item_size),
         p_item, p_dynamic_array->item_size);

  p_dynamic_array->item_count++;
}

void *dynamic_array__at(const DynamicArray *p_dynamic_array, int index) {
  return (uint8_t *)p_dynamic_array->pp_items + (ptrdiff_t)(index * p_dynamic_array->item_size);
}