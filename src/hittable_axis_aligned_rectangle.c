#include <stdlib.h>

#include "hittable_base.h"
#include "material_base.h"
#include "muon_common.h"
#include "ray.h"

// NOTE - XY Rectangle

typedef struct {
  double x0;
  double x1;
  double y0;
  double y1;
  double k;
  const Material *p_material;
} Hittable_XY_Rectangle;

static bool hittable__xy_rectangle__hit(const Hittable_XY_Rectangle *p_hittable_xy_rectangle,
                                        const Ray *p_ray,
                                        double t_min,
                                        double t_max,
                                        Hit *p_hit) {
  const double t = (p_hittable_xy_rectangle->k - p_ray->position.z) / p_ray->direction.z;

  if (t < t_min || t > t_max) {
    return false;
  }

  const double x = p_ray->position.x + t * p_ray->direction.x;
  const double y = p_ray->position.y + t * p_ray->direction.y;

  if (x < p_hittable_xy_rectangle->x0 || x > p_hittable_xy_rectangle->x1 ||
      y < p_hittable_xy_rectangle->y0 || y > p_hittable_xy_rectangle->y1) {
    return false;
  }

  p_hit->t = t;
  p_hit->position = ray__at(p_ray, t);

  const V3 outward_normal = {0.0, 0.0, 1.0};

  p_hit->is_front = v3__dot(p_ray->direction, outward_normal) < 0.0;
  p_hit->normal = p_hit->is_front ? outward_normal : v3__neg(outward_normal);
  p_hit->p_material = p_hittable_xy_rectangle->p_material;

  p_hit->u = (x - p_hittable_xy_rectangle->x0) /
             (p_hittable_xy_rectangle->x1 - p_hittable_xy_rectangle->x0);
  p_hit->v = (y - p_hittable_xy_rectangle->y0) /
             (p_hittable_xy_rectangle->y1 - p_hittable_xy_rectangle->y0);

  return true;
}

static bool hittable__xy_rectangle__bounding_box(
    const Hittable_XY_Rectangle *p_hittable_xy_rectangle, double t0, double t1, AABB *p_aabb) {
  *p_aabb =
      (AABB){.position_minimum = (P3){p_hittable_xy_rectangle->x0, p_hittable_xy_rectangle->y0,
                                      p_hittable_xy_rectangle->k - 0.0001},
             .position_maximum = (P3){p_hittable_xy_rectangle->x1, p_hittable_xy_rectangle->y1,
                                      p_hittable_xy_rectangle->k + 0.0001}};

  return true;
}

static const HittableInterface *p_xy_rectangle_hittable_interface = &(HittableInterface){
    .fp_hit =
        (bool (*)(const void *, const Ray *, double, double, Hit *))hittable__xy_rectangle__hit,
    .fp_bounding_box =
        (bool (*)(const void *, double, double, AABB *))hittable__xy_rectangle__bounding_box};

const Hittable *hittable__xy_rectangle__build(MemoryArena *p_memory_arena,
                                              double x0,
                                              double x1,
                                              double y0,
                                              double y1,
                                              double k,
                                              const Material *p_material) {
  Hittable_XY_Rectangle *p_hittable_xy_rectangle = (Hittable_XY_Rectangle *)memory_arena__allocate(
      p_memory_arena, sizeof(Hittable_XY_Rectangle));

  p_hittable_xy_rectangle->x0 = x0;
  p_hittable_xy_rectangle->x1 = x1;
  p_hittable_xy_rectangle->y0 = y0;
  p_hittable_xy_rectangle->y1 = y1;
  p_hittable_xy_rectangle->k = k;
  p_hittable_xy_rectangle->p_material = p_material;

  return hittable__build(p_memory_arena, p_hittable_xy_rectangle,
                         p_xy_rectangle_hittable_interface);
}

// NOTE - XZ Rectangle

typedef struct {
  double x0;
  double x1;
  double z0;
  double z1;
  double k;
  const Material *p_material;
} Hittable_XZ_Rectangle;

static bool hittable__xz_rectangle__hit(const Hittable_XZ_Rectangle *p_hittable_xz_rectangle,
                                        const Ray *p_ray,
                                        double t_min,
                                        double t_max,
                                        Hit *p_hit) {
  const double t = (p_hittable_xz_rectangle->k - p_ray->position.y) / p_ray->direction.y;

  if (t < t_min || t > t_max) {
    return false;
  }

  const double x = p_ray->position.x + t * p_ray->direction.x;
  const double z = p_ray->position.z + t * p_ray->direction.z;

  if (x < p_hittable_xz_rectangle->x0 || x > p_hittable_xz_rectangle->x1 ||
      z < p_hittable_xz_rectangle->z0 || z > p_hittable_xz_rectangle->z1) {
    return false;
  }

  p_hit->t = t;
  p_hit->position = ray__at(p_ray, t);

  const V3 outward_normal = {0.0, 1.0, 0.0};

  p_hit->is_front = v3__dot(p_ray->direction, outward_normal) < 0.0;
  p_hit->normal = p_hit->is_front ? outward_normal : v3__neg(outward_normal);
  p_hit->p_material = p_hittable_xz_rectangle->p_material;

  p_hit->u = (x - p_hittable_xz_rectangle->x0) /
             (p_hittable_xz_rectangle->x1 - p_hittable_xz_rectangle->x0);
  p_hit->v = (z - p_hittable_xz_rectangle->z0) /
             (p_hittable_xz_rectangle->z1 - p_hittable_xz_rectangle->z0);

  return true;
}

static bool hittable__xz_rectangle__bounding_box(
    const Hittable_XZ_Rectangle *p_hittable_xz_rectangle, double t0, double t1, AABB *p_aabb) {
  *p_aabb = (AABB){.position_minimum =
                       (P3){
                           p_hittable_xz_rectangle->x0,
                           p_hittable_xz_rectangle->k - 0.0001,
                           p_hittable_xz_rectangle->z0,
                       },
                   .position_maximum =
                       (P3){p_hittable_xz_rectangle->x1, p_hittable_xz_rectangle->k + 0.0001,
                            p_hittable_xz_rectangle->z1}};

  return true;
}

static const HittableInterface *p_xz_rectangle_hittable_interface = &(HittableInterface){
    .fp_hit =
        (bool (*)(const void *, const Ray *, double, double, Hit *))hittable__xz_rectangle__hit,
    .fp_bounding_box =
        (bool (*)(const void *, double, double, AABB *))hittable__xz_rectangle__bounding_box};

const Hittable *hittable__xz_rectangle__build(MemoryArena *p_memory_arena,
                                              double x0,
                                              double x1,
                                              double z0,
                                              double z1,
                                              double k,
                                              const Material *p_material) {
  Hittable_XZ_Rectangle *p_hittable_xz_rectangle = (Hittable_XZ_Rectangle *)memory_arena__allocate(
      p_memory_arena, sizeof(Hittable_XZ_Rectangle));

  p_hittable_xz_rectangle->x0 = x0;
  p_hittable_xz_rectangle->x1 = x1;
  p_hittable_xz_rectangle->z0 = z0;
  p_hittable_xz_rectangle->z1 = z1;
  p_hittable_xz_rectangle->k = k;
  p_hittable_xz_rectangle->p_material = p_material;

  return hittable__build(p_memory_arena, p_hittable_xz_rectangle,
                         p_xz_rectangle_hittable_interface);
}

// NOTE - YZ Rectangle

typedef struct {
  double y0;
  double y1;
  double z0;
  double z1;
  double k;
  const Material *p_material;
} Hittable_YZ_Rectangle;

static bool hittable__yz_rectangle__hit(const Hittable_YZ_Rectangle *p_hittable_yz_rectangle,
                                        const Ray *p_ray,
                                        double t_min,
                                        double t_max,
                                        Hit *p_hit) {
  const double t = (p_hittable_yz_rectangle->k - p_ray->position.x) / p_ray->direction.x;

  if (t < t_min || t > t_max) {
    return false;
  }

  const double y = p_ray->position.y + t * p_ray->direction.y;
  const double z = p_ray->position.z + t * p_ray->direction.z;

  if (y < p_hittable_yz_rectangle->y0 || y > p_hittable_yz_rectangle->y1 ||
      z < p_hittable_yz_rectangle->z0 || z > p_hittable_yz_rectangle->z1) {
    return false;
  }

  p_hit->t = t;
  p_hit->position = ray__at(p_ray, t);

  const V3 outward_normal = {1.0, 0.0, 0.0};

  p_hit->is_front = v3__dot(p_ray->direction, outward_normal) < 0.0;
  p_hit->normal = p_hit->is_front ? outward_normal : v3__neg(outward_normal);
  p_hit->p_material = p_hittable_yz_rectangle->p_material;

  p_hit->u = (y - p_hittable_yz_rectangle->y0) /
             (p_hittable_yz_rectangle->y1 - p_hittable_yz_rectangle->y0);
  p_hit->v = (z - p_hittable_yz_rectangle->z0) /
             (p_hittable_yz_rectangle->z1 - p_hittable_yz_rectangle->z0);

  return true;
}

static bool hittable__yz_rectangle__bounding_box(
    const Hittable_YZ_Rectangle *p_hittable_yz_rectangle, double t0, double t1, AABB *p_aabb) {
  *p_aabb =
      (AABB){.position_minimum =
                 (P3){
                     p_hittable_yz_rectangle->k - 0.0001,
                     p_hittable_yz_rectangle->y0,
                     p_hittable_yz_rectangle->z0,
                 },
             .position_maximum = (P3){p_hittable_yz_rectangle->k + 0.0001,
                                      p_hittable_yz_rectangle->y1, p_hittable_yz_rectangle->z1}};

  return true;
}

static const HittableInterface *p_yz_rectangle_hittable_interface = &(HittableInterface){
    .fp_hit =
        (bool (*)(const void *, const Ray *, double, double, Hit *))hittable__yz_rectangle__hit,
    .fp_bounding_box =
        (bool (*)(const void *, double, double, AABB *))hittable__yz_rectangle__bounding_box};

const Hittable *hittable__yz_rectangle__build(MemoryArena *p_memory_arena,
                                              double y0,
                                              double y1,
                                              double z0,
                                              double z1,
                                              double k,
                                              const Material *p_material) {
  Hittable_YZ_Rectangle *p_hittable_yz_rectangle = (Hittable_YZ_Rectangle *)memory_arena__allocate(
      p_memory_arena, sizeof(Hittable_YZ_Rectangle));

  p_hittable_yz_rectangle->y0 = y0;
  p_hittable_yz_rectangle->y1 = y1;
  p_hittable_yz_rectangle->z0 = z0;
  p_hittable_yz_rectangle->z1 = z1;
  p_hittable_yz_rectangle->k = k;
  p_hittable_yz_rectangle->p_material = p_material;

  return hittable__build(p_memory_arena, p_hittable_yz_rectangle,
                         p_yz_rectangle_hittable_interface);
}