#include <stdbool.h>

#include "axis_aligned_bounding_box.h"
#include "hittable_base.h"
#include "muon_common.h"
#include "ray.h"

const Hittable *hittable__build(MemoryArena *p_memory_arena,
                                const void *p_instance,
                                const HittableInterface *p_interface) {
  Hittable *p_hittable = (Hittable *)memory_arena__allocate(p_memory_arena, sizeof(Hittable));

  p_hittable->p_instance = p_instance;
  p_hittable->p_interface = p_interface;

  return p_hittable;
}

bool hittable__hit(
    const Hittable *p_hittable, const Ray *p_ray, double t_min, double t_max, Hit *p_hit) {
  return (p_hittable->p_interface->fp_hit)(p_hittable->p_instance, p_ray, t_min, t_max, p_hit);
}

bool hittable__bounding_box(const Hittable *p_hittable, double t0, double t1, AABB *p_aabb) {
  return (p_hittable->p_interface->fp_bounding_box)(p_hittable->p_instance, t0, t1, p_aabb);
}