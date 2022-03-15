#include <stdbool.h>

#include "hittable_base.h"
#include "material_base.h"
#include "muon_common.h"
#include "ray.h"
#include "texture_base.h"

typedef struct {
  const Texture *p_texture;
} Material_Lambertian;

static bool material__lambertian__scatter(const Material_Lambertian *p_material_lambertian,
                                          const Ray *p_ray_in,
                                          const Hit *p_hit,
                                          Ray *p_ray_out,
                                          C3 *p_attenuation) {
  V3 direction = v3__add(p_hit->normal, v3__random_in_sphere(1.0));

  if (v3__is_near_zero(direction)) {
    direction = p_hit->normal;
  }

  *p_ray_out = (Ray){.position = p_hit->position, .direction = direction, .time = p_ray_in->time};

  *p_attenuation =
      texture__value(p_material_lambertian->p_texture, p_hit->u, p_hit->v, p_hit->position);

  return true;
}

static const MaterialInterface *p_lambertian_material_interface = &(MaterialInterface){
    .fp_scatter = (bool (*)(
        const void *, const Ray *, const Hit *, Ray *, C3 *))material__lambertian__scatter,
    .fp_emit = 0};

const Material *material__lambertian__build(MemoryArena *p_memory_arena, const Texture *p_texture) {
  Material_Lambertian *p_instance =
      (Material_Lambertian *)memory_arena__allocate(p_memory_arena, sizeof(Material_Lambertian));

  p_instance->p_texture = p_texture;

  return material__build(p_memory_arena, p_instance, p_lambertian_material_interface);
}
