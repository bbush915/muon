#include <stdbool.h>

#include "axis_aligned_bounding_box.h"
#include "hittable_base.h"
#include "hittable_translation.h"
#include "muon_common.h"
#include "ray.h"

typedef struct {
  const Hittable *p_hittable;
  V3 offset;
} Hittable_Translation;

static bool hittable__translation__hit(const Hittable_Translation *p_hittable_translation,
                                       const Ray *p_ray,
                                       double t_min,
                                       double t_max,
                                       Hit *p_hit) {
  const Ray translated_ray =
      (Ray){.position = v3__sub(p_ray->position, p_hittable_translation->offset),
            .direction = p_ray->direction,
            .time = p_ray->time};

  if (!hittable__hit(p_hittable_translation->p_hittable, &translated_ray, t_min, t_max, p_hit)) {
    return false;
  }

  p_hit->position = v3__add(p_hit->position, p_hittable_translation->offset);

  p_hit->is_front = v3__dot(translated_ray.direction, p_hit->normal) < 0.0;
  p_hit->normal = p_hit->is_front ? p_hit->normal : v3__neg(p_hit->normal);

  return true;
}

static bool hittable__translation__bounding_box(const Hittable_Translation *p_hittable_translation,
                                                double t0,
                                                double t1,
                                                AABB *p_aabb) {
  if (!hittable__bounding_box(p_hittable_translation->p_hittable, t0, t1, p_aabb)) {
    return false;
  }

  *p_aabb =
      (AABB){.position_minimum = v3__add(p_aabb->position_minimum, p_hittable_translation->offset),
             .position_maximum = v3__add(p_aabb->position_maximum, p_hittable_translation->offset)};

  return true;
}

static const HittableInterface *p_translation_hittable_interface = &(HittableInterface){
    .fp_hit =
        (bool (*)(const void *, const Ray *, double, double, Hit *))hittable__translation__hit,
    .fp_bounding_box =
        (bool (*)(const void *, double, double, AABB *))hittable__translation__bounding_box};

const Hittable *
hittable__translation__build(MemoryArena *p_memory_arena, const Hittable *p_hittable, V3 offset) {
  Hittable_Translation *p_instance =
      (Hittable_Translation *)memory_arena__allocate(p_memory_arena, sizeof(Hittable_Translation));

  p_instance->p_hittable = p_hittable;
  p_instance->offset = offset;

  return hittable__build(p_memory_arena, p_instance, p_translation_hittable_interface);
}
