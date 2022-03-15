#include <math.h>
#include <stdbool.h>

#include "axis_aligned_bounding_box.h"
#include "hittable_base.h"
#include "hittable_sphere.h"
#include "material_base.h"
#include "muon_common.h"
#include "ray.h"

typedef struct {
  const Hittable *p_hittable;
  double density;
  const Material *p_material;
} Hittable_Constant_Medium;

static bool
hittable__constant_medium__hit(const Hittable_Constant_Medium *p_hittable_constant_medium,
                               const Ray *p_ray,
                               double t_min,
                               double t_max,
                               Hit *p_hit) {
  Hit hit1;

  if (!hittable__hit(p_hittable_constant_medium->p_hittable, p_ray, -INFINITY, INFINITY, &hit1)) {
    return false;
  }

  Hit hit2;

  if (!hittable__hit(p_hittable_constant_medium->p_hittable, p_ray, hit1.t + 0.001, INFINITY,
                     &hit2)) {
    return false;
  }

  if (hit1.t < t_min) {
    hit1.t = t_min;
  }

  if (hit2.t > t_max) {
    hit2.t = t_max;
  }

  if (hit1.t >= hit2.t) {
    return false;
  }

  if (hit1.t < 0) {
    hit1.t = 0;
  }

  const double ray_length = v3__len(p_ray->direction);
  const double distance_inside_boundary = (hit2.t - hit1.t) * ray_length;
  const double hit_distance =
      (-1.0 / p_hittable_constant_medium->density) * log(utils__random_double());

  if (hit_distance > distance_inside_boundary) {
    return false;
  }

  p_hit->t = hit1.t + hit_distance / ray_length;
  p_hit->position = ray__at(p_ray, p_hit->t);

  p_hit->is_front = true;
  p_hit->normal = (V3){1.0, 0.0, 0.0};
  p_hit->p_material = p_hittable_constant_medium->p_material;

  return true;
}

static bool
hittable__constant_medium__bounding_box(const Hittable_Constant_Medium *p_hittable_constant_medium,
                                        double t0,
                                        double t1,
                                        AABB *p_aabb) {
  return hittable__bounding_box(p_hittable_constant_medium->p_hittable, t0, t1, p_aabb);
}

static const HittableInterface *p_constant_medium_hittable_interface = &(HittableInterface){
    .fp_hit =
        (bool (*)(const void *, const Ray *, double, double, Hit *))hittable__constant_medium__hit,
    .fp_bounding_box =
        (bool (*)(const void *, double, double, AABB *))hittable__constant_medium__bounding_box};

const Hittable *hittable__constant_medium__build(MemoryArena *p_memory_arena,
                                                 const Hittable *p_hittable,
                                                 double density,
                                                 const Material *p_material) {
  Hittable_Constant_Medium *p_instance = (Hittable_Constant_Medium *)memory_arena__allocate(
      p_memory_arena, sizeof(Hittable_Constant_Medium));

  p_instance->p_hittable = p_hittable;
  p_instance->density = density;
  p_instance->p_material = p_material;

  return hittable__build(p_memory_arena, p_instance, p_constant_medium_hittable_interface);
}