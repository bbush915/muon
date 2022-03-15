#include "hittable_bounding_volume_hierarchy.h"
#include "axis_aligned_bounding_box.h"
#include "hittable_base.h"
#include "muon_common.h"
#include "ray.h"

typedef struct {
  const Hittable *p_hittable_left;
  const Hittable *p_hittable_right;
  AABB boundng_box;
} Hittable_BVH;

static int hittable__bounding_box__compare(const Hittable *p_a, const Hittable *p_b, int axis) {
  AABB bounding_box_a;
  AABB bounding_box_b;

  hittable__bounding_box(p_a, 0, 0, &bounding_box_a);
  hittable__bounding_box(p_b, 0, 0, &bounding_box_b);

  const double comparison = bounding_box_a.position_minimum.components[axis] -
                            bounding_box_b.position_minimum.components[axis];

  return comparison <= 0 ? -1 : 1;
}

int hittable__bounding_box__compare_x(const void *p_a, const void *p_b) {
  return hittable__bounding_box__compare(*(Hittable **)p_a, *(Hittable **)p_b, 0);
}

int hittable__bounding_box__compare_y(const void *p_a, const void *p_b) {
  return hittable__bounding_box__compare(*(Hittable **)p_a, *(Hittable **)p_b, 1);
}

int hittable__bounding_box__compare_z(const void *p_a, const void *p_b) {
  return hittable__bounding_box__compare(*(Hittable **)p_a, *(Hittable **)p_b, 2);
}

static bool hittable__bvh__hit(
    const Hittable_BVH *p_hittable_bvh, const Ray *p_ray, double t_min, double t_max, Hit *p_hit) {
  if (!aabb__hit(&p_hittable_bvh->boundng_box, p_ray, t_min, t_max)) {
    return false;
  }

  const bool hit_left = hittable__hit(p_hittable_bvh->p_hittable_left, p_ray, t_min, t_max, p_hit);
  const bool hit_right = hittable__hit(p_hittable_bvh->p_hittable_right, p_ray, t_min,
                                       hit_left ? p_hit->t : t_max, p_hit);

  if (hit_left || hit_right) {
    const int x = 3;
  }

  return hit_left || hit_right;
}

static bool hittable__bvh__bounding_box(const Hittable_BVH *p_hittable_bvh,
                                        double t0,
                                        double t1,
                                        AABB *p_aabb) {
  *p_aabb = p_hittable_bvh->boundng_box;

  return true;
}

static const HittableInterface *p_bvh_hittable_interface = &(HittableInterface){
    .fp_hit = (bool (*)(const void *, const Ray *, double, double, Hit *))hittable__bvh__hit,
    .fp_bounding_box = (bool (*)(const void *, double, double, AABB *))hittable__bvh__bounding_box};

const Hittable *hittable__bounding_volume_hierarchy__build(MemoryArena *p_memory_arena,
                                                           const Hittable **pp_hittables,
                                                           int hittable_count,
                                                           int index_start,
                                                           int index_finish,
                                                           double t0,
                                                           double t1) {
  Hittable_BVH *p_instance =
      (Hittable_BVH *)memory_arena__allocate(p_memory_arena, sizeof(Hittable_BVH));

  const int axis = utils__random_int_in_range(0, 2);

  int (*comparator)(const void *, const void *) = (axis == 0)   ? hittable__bounding_box__compare_x
                                                  : (axis == 1) ? hittable__bounding_box__compare_y
                                                                : hittable__bounding_box__compare_z;

  const int index_span = index_finish - index_start;

  switch (index_span) {
    case 1: {
      // NOTE - If there is only one element, place it on each side.

      p_instance->p_hittable_left = pp_hittables[index_start];
      p_instance->p_hittable_right = pp_hittables[index_start];

      break;
    }

    case 2: {
      // NOTE - If there are two elements, directly compare them and place them
      // accordingly.

      const int comparison = comparator(&pp_hittables[index_start], &pp_hittables[index_start + 1]);

      if (comparison <= 0) {
        p_instance->p_hittable_left = pp_hittables[index_start];
        p_instance->p_hittable_right = pp_hittables[index_start + 1];
      } else {
        p_instance->p_hittable_left = pp_hittables[index_start + 1];
        p_instance->p_hittable_right = pp_hittables[index_start];
      }

      break;
    }

    default: {
      // NOTE - Otherwise, we sort the relevant elements and recurse.

      qsort(pp_hittables + index_start, index_span, sizeof(Hittable *), comparator);

      const int index_middle = index_start + index_span / 2;

      p_instance->p_hittable_left = hittable__bounding_volume_hierarchy__build(
          p_memory_arena, pp_hittables, hittable_count, index_start, index_middle, t0, t1);
      p_instance->p_hittable_right = hittable__bounding_volume_hierarchy__build(
          p_memory_arena, pp_hittables, hittable_count, index_middle, index_finish, t0, t1);

      break;
    }
  }

  AABB bounding_box_left;
  AABB bounding_box_right;

  hittable__bounding_box(p_instance->p_hittable_left, t0, t1, &bounding_box_left);
  hittable__bounding_box(p_instance->p_hittable_right, t0, t1, &bounding_box_right);

  p_instance->boundng_box = aabb__surrounding_bounding_box(bounding_box_left, bounding_box_right);

  return hittable__build(p_memory_arena, p_instance, p_bvh_hittable_interface);
}