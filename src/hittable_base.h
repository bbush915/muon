#ifndef HITTABLE_BASE_H
#define HITTABLE_BASE_H

#include <stdbool.h>

typedef struct Hittable_s Hittable;
typedef struct HittableInterface_s HittableInterface;
typedef struct Hit_s Hit;

#include "axis_aligned_bounding_box.h"
#include "material_base.h"
#include "muon_common.h"
#include "ray.h"

struct Hittable_s {
  const void *p_instance;
  const HittableInterface *p_interface;
};

const Hittable *hittable__build(MemoryArena *p_memory_arena,
                                const void *p_instance,
                                const HittableInterface *p_interface);

struct HittableInterface_s {
  bool (*const fp_hit)(
      const void *p_instance, const Ray *p_ray, double t_min, double t_max, Hit *p_hit);
  bool (*const fp_bounding_box)(const void *p_instance, double t0, double t1, AABB *p_aabb);
};

bool hittable__hit(
    const Hittable *p_hittable, const Ray *p_ray, double t_min, double t_max, Hit *p_hit);

bool hittable__bounding_box(const Hittable *p_hittable, double t0, double t1, AABB *p_aabb);

struct Hit_s {
  double t;
  P3 position;
  V3 normal;
  bool is_front;
  const Material *p_material;
  double u;
  double v;
};

#endif