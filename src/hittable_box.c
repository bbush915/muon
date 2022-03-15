#include <stdbool.h>

#include "axis_aligned_bounding_box.h"
#include "hittable_base.h"
#include "hittable_box.h"
#include "hittable_list.h"
#include "material_base.h"
#include "muon_common.h"
#include "ray.h"

typedef struct {
  P3 position_minimum;
  P3 position_maximum;
  const Hittable *p_hittable_list;
  const Material *p_material;
} Hittable_Box;

static bool hittable__box__hit(
    const Hittable_Box *p_hittable_box, const Ray *p_ray, double t_min, double t_max, Hit *p_hit) {
  return hittable__hit(p_hittable_box->p_hittable_list, p_ray, t_min, t_max, p_hit);
}

static bool hittable__box__bounding_box(const Hittable_Box *p_hittable_box,
                                        double t0,
                                        double t1,
                                        AABB *p_aabb) {
  *p_aabb = (AABB){.position_minimum = p_hittable_box->position_minimum,
                   .position_maximum = p_hittable_box->position_maximum};

  return true;
}

static const HittableInterface *p_box_hittable_interface = &(HittableInterface){
    .fp_hit = (bool (*)(const void *, const Ray *, double, double, Hit *))hittable__box__hit,
    .fp_bounding_box = (bool (*)(const void *, double, double, AABB *))hittable__box__bounding_box};

const Hittable *hittable__box__build(MemoryArena *p_memory_arena,
                                     P3 position_minimum,
                                     P3 position_maximum,
                                     const Material *p_material) {
  Hittable_Box *p_instance =
      (Hittable_Box *)memory_arena__allocate(p_memory_arena, sizeof(Hittable_Box));

  p_instance->position_minimum = position_minimum;
  p_instance->position_maximum = position_maximum;
  p_instance->p_material = p_material;

  const Hittable *p_front = hittable__xy_rectangle__build(
      p_memory_arena, position_minimum.x, position_maximum.x, position_minimum.y,
      position_maximum.y, position_maximum.z, p_material);

  const Hittable *p_back = hittable__xy_rectangle__build(
      p_memory_arena, position_minimum.x, position_maximum.x, position_minimum.y,
      position_maximum.y, position_minimum.z, p_material);

  const Hittable *p_top = hittable__xz_rectangle__build(
      p_memory_arena, position_minimum.x, position_maximum.x, position_minimum.z,
      position_maximum.z, position_maximum.y, p_material);

  const Hittable *p_bottom = hittable__xz_rectangle__build(
      p_memory_arena, position_minimum.x, position_maximum.x, position_minimum.z,
      position_maximum.z, position_minimum.y, p_material);

  const Hittable *p_right = hittable__yz_rectangle__build(
      p_memory_arena, position_minimum.y, position_maximum.y, position_minimum.z,
      position_maximum.z, position_maximum.x, p_material);

  const Hittable *p_left = hittable__yz_rectangle__build(
      p_memory_arena, position_minimum.y, position_maximum.y, position_minimum.z,
      position_maximum.z, position_minimum.x, p_material);

  const int hittable_count = 6;
  const Hittable **pp_hittables = (const Hittable **)memory_arena__allocate(
      p_memory_arena, hittable_count * sizeof(Hittable *));

  pp_hittables[0] = p_front;
  pp_hittables[1] = p_back;
  pp_hittables[2] = p_top;
  pp_hittables[3] = p_bottom;
  pp_hittables[4] = p_left;
  pp_hittables[5] = p_right;

  p_instance->p_hittable_list = hittable__list__build(p_memory_arena, pp_hittables, hittable_count);

  return hittable__build(p_memory_arena, p_instance, p_box_hittable_interface);
}