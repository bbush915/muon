#include <math.h>
#include <stdbool.h>

#include "hittable_base.h"
#include "material_base.h"
#include "muon_common.h"
#include "ray.h"
#include "texture_base.h"

typedef struct {
  double refractive_index;
} Material_Dialectric;

static bool material__dialectric__scatter(const Material_Dialectric *p_material_dialectric,
                                          const Ray *p_ray_in,
                                          const Hit *p_hit,
                                          Ray *p_ray_out,
                                          C3 *p_attenuation) {
  const double refractive_index_ratio = p_hit->is_front
                                            ? (1.0 / p_material_dialectric->refractive_index)
                                            : p_material_dialectric->refractive_index;

  const V3 unit_direction = v3__unit(p_ray_in->direction);

  const double cos_theta = fmin(v3__dot(v3__neg(unit_direction), p_hit->normal), 1.0);
  const double sin_theta = sqrt(1.0 - cos_theta * cos_theta);

  const bool is_total_internal_reflection = refractive_index_ratio * sin_theta > 1.0;

  // NOTE - Use Schlick's approximation to calculate reflectance.

  const double r0 = pow((1.0 - refractive_index_ratio) / (1.0 + refractive_index_ratio), 2.0);
  const double reflectance = r0 + (1.0 - r0) * pow(1.0 - cos_theta, 5.0);

  const V3 direction = is_total_internal_reflection || reflectance > utils__random_double()
                           ? v3__reflect(unit_direction, p_hit->normal)
                           : v3__refract(unit_direction, p_hit->normal, refractive_index_ratio);

  *p_ray_out = (Ray){.position = p_hit->position, .direction = direction, .time = p_ray_in->time};

  *p_attenuation = (C3){
      1.0,
      1.0,
      1.0,
  };

  return true;
}

static const MaterialInterface *p_dialectric_material_interface = &(MaterialInterface){
    .fp_scatter = (bool (*)(
        const void *, const Ray *, const Hit *, Ray *, C3 *))material__dialectric__scatter,
    .fp_emit = 0};

const Material *material__dialectric__build(MemoryArena *p_memory_arena, double refractive_index) {
  Material_Dialectric *p_instance =
      (Material_Dialectric *)memory_arena__allocate(p_memory_arena, sizeof(Material_Dialectric));

  p_instance->refractive_index = refractive_index;

  return material__build(p_memory_arena, p_instance, p_dialectric_material_interface);
}