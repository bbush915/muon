#include <math.h>
#include <stdbool.h>

#include "axis_aligned_bounding_box.h"
#include "hittable_base.h"
#include "hittable_rotation.h"
#include "material_base.h"
#include "muon_common.h"
#include "ray.h"

typedef struct {
  const Hittable *p_hittable;
  double sin_theta;
  double cos_theta;
  bool has_aabb;
  AABB aabb;
} Hittable_Y_Rotation;

static bool hittable__y_rotation__hit(const Hittable_Y_Rotation *p_hittable_y_rotation,
                                      const Ray *p_ray,
                                      double t_min,
                                      double t_max,
                                      Hit *p_hit) {
  P3 ray_position = p_ray->position;
  V3 ray_direction = p_ray->direction;

  ray_position.x = p_hittable_y_rotation->cos_theta * p_ray->position.x -
                   p_hittable_y_rotation->sin_theta * p_ray->position.z;
  ray_position.z = p_hittable_y_rotation->sin_theta * p_ray->position.x +
                   p_hittable_y_rotation->cos_theta * p_ray->position.z;

  ray_direction.x = p_hittable_y_rotation->cos_theta * p_ray->direction.x -
                    p_hittable_y_rotation->sin_theta * p_ray->direction.z;
  ray_direction.z = p_hittable_y_rotation->sin_theta * p_ray->direction.x +
                    p_hittable_y_rotation->cos_theta * p_ray->direction.z;

  const Ray rotated_ray =
      (Ray){.position = ray_position, .direction = ray_direction, .time = p_ray->time};

  if (!hittable__hit(p_hittable_y_rotation->p_hittable, &rotated_ray, t_min, t_max, p_hit)) {
    return false;
  }

  P3 hit_position = p_hit->position;
  V3 hit_normal = p_hit->normal;

  hit_position.x = p_hittable_y_rotation->cos_theta * p_hit->position.x +
                   p_hittable_y_rotation->sin_theta * p_hit->position.z;
  hit_position.z = -p_hittable_y_rotation->sin_theta * p_hit->position.x +
                   p_hittable_y_rotation->cos_theta * p_hit->position.z;

  hit_normal.x = p_hittable_y_rotation->cos_theta * p_hit->normal.x +
                 p_hittable_y_rotation->sin_theta * p_hit->normal.z;
  hit_normal.z = -p_hittable_y_rotation->sin_theta * p_hit->normal.x +
                 p_hittable_y_rotation->cos_theta * p_hit->normal.z;

  p_hit->position = hit_position;

  p_hit->is_front = v3__dot(rotated_ray.direction, hit_normal) < 0.0;
  p_hit->normal = p_hit->is_front ? hit_normal : v3__neg(hit_normal);

  return true;
}

static bool hittable__y_rotation__bounding_box(const Hittable_Y_Rotation *p_hittable_y_rotation,
                                               double t0,
                                               double t1,
                                               AABB *p_aabb) {
  *p_aabb = p_hittable_y_rotation->aabb;

  return p_hittable_y_rotation->has_aabb;
}

static const HittableInterface *p_y_rotation_hittable_interface = &(HittableInterface){
    .fp_hit = (bool (*)(const void *, const Ray *, double, double, Hit *))hittable__y_rotation__hit,
    .fp_bounding_box =
        (bool (*)(const void *, double, double, AABB *))hittable__y_rotation__bounding_box};

const Hittable *hittable__y_rotation__build(MemoryArena *p_memory_arena,
                                            const Hittable *p_hittable,
                                            double degrees) {
  Hittable_Y_Rotation *p_instance =
      (Hittable_Y_Rotation *)memory_arena__allocate(p_memory_arena, sizeof(Hittable_Y_Rotation));

  p_instance->p_hittable = p_hittable;

  const double radians = utils__degrees_to_radians(degrees);

  p_instance->sin_theta = sin(radians);
  p_instance->cos_theta = cos(radians);

  p_instance->has_aabb = hittable__bounding_box(p_hittable, 0.0, 1.0, &p_instance->aabb);

  P3 position_minimum = {INFINITY, INFINITY, INFINITY};
  P3 position_maximum = {-INFINITY, -INFINITY, -INFINITY};

  for (int i = 0; i < 2; i++) {
    for (int j = 0; j < 2; j++) {
      for (int k = 0; k < 2; k++) {
        const double x =
            i * p_instance->aabb.position_maximum.x + (1 - i) * p_instance->aabb.position_minimum.x;
        const double y =
            j * p_instance->aabb.position_maximum.y + (1 - j) * p_instance->aabb.position_minimum.y;
        const double z =
            k * p_instance->aabb.position_maximum.z + (1 - k) * p_instance->aabb.position_minimum.z;

        const double new_x = p_instance->cos_theta * x + p_instance->sin_theta * z;
        const double new_z = -p_instance->sin_theta * x + p_instance->cos_theta * z;

        V3 probe = {new_x, y, new_z};

        for (int n = 0; n < 3; n++) {
          position_minimum.components[n] =
              fmin(position_minimum.components[n], probe.components[n]);
          position_maximum.components[n] =
              fmax(position_maximum.components[n], probe.components[n]);
        }
      }
    }
  }

  p_instance->aabb =
      (AABB){.position_minimum = position_minimum, .position_maximum = position_maximum};

  return hittable__build(p_memory_arena, p_instance, p_y_rotation_hittable_interface);
}