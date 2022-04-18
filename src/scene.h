#ifndef SCENE_H
#define SCENE_H

typedef struct Scene_s Scene;

#include "camera.h"
#include "hittable_base.h"
#include "muon_common.h"

struct Scene_s {
  double aspect_ratio;
  int height;
  int width;
  int samples_per_pixel;
  int max_depth;
  C3 background_color;
  const Camera *p_camera;
  const Hittable *p_hittables;
};

const Scene *scene__final_rt_weekend__build(MemoryArena *p_memory_arena);

const Scene *scene__cornell_box__build(MemoryArena *p_memory_arena);

const Scene *scene__final_rt_next_week__build(MemoryArena *p_memory_arena);

const Scene *scene__waves__build(MemoryArena *p_memory_arena);

const Scene *scene__planets__build(MemoryArena *p_memory_arena);

#endif