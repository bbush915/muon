#include <math.h>
#include <stdbool.h>

#include "axis_aligned_bounding_box.h"
#include "hittable_base.h"
#include "hittable_sphere.h"
#include "material_base.h"
#include "muon_common.h"
#include "ray.h"

typedef struct {
  P3 position_start;
  P3 position_finish;
  double time_start;
  double time_finish;
  double radius;
  const Material *p_material;
} Hittable_Sphere;

static P3 hittable__sphere__position(const Hittable_Sphere *p_hittable_sphere, double t) {
  // NOTE - Linear interpolation of the sphere position at the given time.

  const double coefficient = (t - p_hittable_sphere->time_start) /
                             (p_hittable_sphere->time_finish - p_hittable_sphere->time_start);

  return v3__add(
      p_hittable_sphere->position_start,
      v3__mul(v3__sub(p_hittable_sphere->position_finish, p_hittable_sphere->position_start),
              coefficient));
}

static void hittable__sphere__uv(P3 position, double *p_u, double *p_v) {
  // NOTE - Spherical coordinates.

  const double theta = acos(-position.y);
  const double phi = atan2(-position.z, position.x) + PI;

  *p_u = phi / (2 * PI);
  *p_v = theta / PI;
}

static bool hittable__sphere__hit(const Hittable_Sphere *p_hittable_sphere,
                                  const Ray *p_ray,
                                  double t_min,
                                  double t_max,
                                  Hit *p_hit) {
  const P3 position = hittable__sphere__position(p_hittable_sphere, p_ray->time);
  const V3 normal = v3__sub(p_ray->position, position);

  const double a = v3__len2(p_ray->direction);
  const double b = 2.0 * v3__dot(normal, p_ray->direction);
  const double c = v3__len2(normal) - p_hittable_sphere->radius * p_hittable_sphere->radius;

  const double discriminant = b * b - 4.0 * a * c;

  if (discriminant < 0) {
    return false;
  }

  double t = (-b - sqrt(discriminant)) / (2.0 * a);

  if (t < t_min || t > t_max) {
    t = (-b + sqrt(discriminant)) / (2.0 * a);

    if (t < t_min || t > t_max) {
      return false;
    }
  }

  p_hit->t = t;
  p_hit->position = ray__at(p_ray, p_hit->t);
  p_hit->p_material = p_hittable_sphere->p_material;

  const V3 outward_normal = v3__div(v3__sub(p_hit->position, position), p_hittable_sphere->radius);

  p_hit->is_front = v3__dot(p_ray->direction, outward_normal) < 0.0;
  p_hit->normal = p_hit->is_front ? outward_normal : v3__neg(outward_normal);

  hittable__sphere__uv(outward_normal, &p_hit->u, &p_hit->v);

  return true;
}

static bool hittable__sphere__bounding_box(const Hittable_Sphere *p_hittable_sphere,
                                           double t0,
                                           double t1,
                                           AABB *p_aabb) {
  const double radius = p_hittable_sphere->radius;
  const V3 offset = (V3){radius, radius, radius};

  const P3 p0 = hittable__sphere__position(p_hittable_sphere, t0);

  const AABB aabb0 = {.position_minimum = v3__sub(p0, offset),
                      .position_maximum = v3__add(p0, offset)};

  const P3 p1 = hittable__sphere__position(p_hittable_sphere, t1);

  const AABB aabb1 = {.position_minimum = v3__sub(p1, offset),
                      .position_maximum = v3__add(p1, offset)};

  *p_aabb = aabb__surrounding_bounding_box(aabb0, aabb1);

  return true;
}

static const HittableInterface *p_sphere_hittable_interface = &(HittableInterface){
    .fp_hit = (bool (*)(const void *, const Ray *, double, double, Hit *))hittable__sphere__hit,
    .fp_bounding_box =
        (bool (*)(const void *, double, double, AABB *))hittable__sphere__bounding_box};

const Hittable *hittable__sphere__build(MemoryArena *p_memory_arena,
                                        P3 position,
                                        double radius,
                                        const Material *p_material) {
  return hittable__moving_sphere__build(p_memory_arena, position, position, 0.0, 1.0, radius,
                                        p_material);
}

const Hittable *hittable__moving_sphere__build(MemoryArena *p_memory_arena,
                                               P3 position_start,
                                               P3 position_finish,
                                               double time_start,
                                               double time_finish,
                                               double radius,
                                               const Material *p_material) {
  Hittable_Sphere *p_instance =
      (Hittable_Sphere *)memory_arena__allocate(p_memory_arena, sizeof(Hittable_Sphere));

  p_instance->position_start = position_start;
  p_instance->position_finish = position_finish;
  p_instance->time_start = time_start;
  p_instance->time_finish = time_finish;
  p_instance->radius = radius;
  p_instance->p_material = p_material;

  return hittable__build(p_memory_arena, p_instance, p_sphere_hittable_interface);
}
