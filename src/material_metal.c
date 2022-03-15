#include <stdbool.h>

#include "hittable_base.h"
#include "material_base.h"
#include "muon_common.h"
#include "ray.h"
#include "texture_base.h"

typedef struct {
  C3 albedo;
  double fuzzing_factor;
} Material_Metal;

static bool material__metal__scatter(const Material_Metal *p_material_metal,
                                     const Ray *p_ray_in,
                                     const Hit *p_hit,
                                     Ray *p_ray_out,
                                     C3 *p_attenuation) {
  const V3 direction = v3__reflect(v3__unit(p_ray_in->direction), p_hit->normal);
  const V3 fuzzed_direction =
      v3__add(direction, v3__mul(v3__random_in_sphere(1.0), p_material_metal->fuzzing_factor));

  *p_ray_out =
      (Ray){.position = p_hit->position, .direction = fuzzed_direction, .time = p_ray_in->time};

  *p_attenuation = p_material_metal->albedo;

  return v3__dot(p_ray_out->direction, p_hit->normal) > 0;
}

static const MaterialInterface *p_metal_material_interface = &(MaterialInterface){
    .fp_scatter =
        (bool (*)(const void *, const Ray *, const Hit *, Ray *, C3 *))material__metal__scatter,
    .fp_emit = 0};

const Material *
material__metal__build(MemoryArena *p_memory_arena, C3 albedo, double fuzzing_factor) {
  Material_Metal *p_instance =
      (Material_Metal *)memory_arena__allocate(p_memory_arena, sizeof(Material_Metal));

  p_instance->albedo = albedo;
  p_instance->fuzzing_factor = fuzzing_factor;

  return material__build(p_memory_arena, p_instance, p_metal_material_interface);
}
