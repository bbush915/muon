#include <stdbool.h>

#include "axis_aligned_bounding_box.h"
#include "hittable_base.h"
#include "hittable_list.h"
#include "muon_common.h"
#include "ray.h"

typedef struct {
  const Hittable **pp_hittables;
  int hittable_count;
} Hittable_List;

static bool hittable__list__hit(const Hittable_List *p_hittable_list,
                                const Ray *p_ray,
                                double t_min,
                                double t_max,
                                Hit *p_hit) {
  Hit hit = {};

  bool did_hit = false;
  double closest_t = t_max;

  for (int i = 0; i < p_hittable_list->hittable_count; i++) {
    const Hittable *p_hittable = p_hittable_list->pp_hittables[i];

    if (hittable__hit(p_hittable, p_ray, t_min, closest_t, &hit)) {
      did_hit = true;
      closest_t = hit.t;

      p_hit->t = hit.t;
      p_hit->position = hit.position;
      p_hit->normal = hit.normal;
      p_hit->is_front = hit.is_front;
      p_hit->p_material = hit.p_material;
      p_hit->u = hit.u;
      p_hit->v = hit.v;
    }
  }

  return did_hit;
}

static bool hittable__list__bounding_box(const Hittable_List *p_hittable_list,
                                         double t0,
                                         double t1,
                                         AABB *p_aabb) {
  if (p_hittable_list->hittable_count == 0) {
    return false;
  }

  AABB aabb;
  bool first_iteration = true;

  for (int i = 0; i < p_hittable_list->hittable_count; i++) {
    const Hittable *p_hittable = p_hittable_list->pp_hittables[i];

    if (!hittable__bounding_box(p_hittable, t0, t1, &aabb)) {
      return false;
    }

    *p_aabb = first_iteration ? aabb : aabb__surrounding_bounding_box(*p_aabb, aabb);

    first_iteration = false;
  }

  return true;
}

static const HittableInterface *p_list_hittable_interface = &(HittableInterface){
    .fp_hit = (bool (*)(const void *, const Ray *, double, double, Hit *))hittable__list__hit,
    .fp_bounding_box =
        (bool (*)(const void *, double, double, AABB *))hittable__list__bounding_box};

const Hittable *hittable__list__build(MemoryArena *p_memory_arena,
                                      const Hittable **pp_hittables,
                                      int hittable_count) {
  Hittable_List *p_instance =
      (Hittable_List *)memory_arena__allocate(p_memory_arena, sizeof(Hittable_List));

  p_instance->pp_hittables = pp_hittables;
  p_instance->hittable_count = hittable_count;

  return hittable__build(p_memory_arena, p_instance, p_list_hittable_interface);
}