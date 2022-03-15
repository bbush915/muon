#include <stdbool.h>

#include "hittable_base.h"
#include "material_base.h"
#include "muon_common.h"
#include "ray.h"

const Material *material__build(MemoryArena *p_memory_arena,
                                const void *p_instance,
                                const MaterialInterface *p_interface) {
  Material *p_material = (Material *)memory_arena__allocate(p_memory_arena, sizeof(Material));

  p_material->p_instance = p_instance;
  p_material->p_interface = p_interface;

  return p_material;
}

bool material__scatter(const Material *p_material,
                       const Ray *p_ray_in,
                       const Hit *p_hit,
                       Ray *p_ray_out,
                       C3 *p_attenuation) {
  return (p_material->p_interface->fp_scatter)(p_material->p_instance, p_ray_in, p_hit, p_ray_out,
                                               p_attenuation);
}

C3 material__emit(const Material *p_material, double u, double v, P3 position) {
  // NOTE - If the material does not implement the emit function, then we
  // return a solid black color.

  if (!p_material->p_interface->fp_emit) {
    return (C3){0.0, 0.0, 0.0};
  }

  return (p_material->p_interface->fp_emit)(p_material->p_instance, u, v, position);
}