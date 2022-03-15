#include <stdbool.h>

#include "material_base.h"
#include "muon_common.h"
#include "ray.h"
#include "texture_base.h"

typedef struct {
  const Texture *p_texture;
} Material_Isotropic;

static bool material__isotropic__scatter(const Material_Isotropic *p_material_isotropic,
                                         const Ray *p_ray_in,
                                         const Hit *p_hit,
                                         Ray *p_ray_out,
                                         C3 *p_attenuation) {
  *p_ray_out = (Ray){
      .position = p_hit->position, .direction = v3__random_in_sphere(1.0), .time = p_ray_in->time};

  *p_attenuation =
      texture__value(p_material_isotropic->p_texture, p_hit->u, p_hit->v, p_hit->position);

  return true;
}

static const MaterialInterface *p_isotropic_material_interface = &(MaterialInterface){
    .fp_scatter =
        (bool (*)(const void *, const Ray *, const Hit *, Ray *, C3 *))material__isotropic__scatter,
    .fp_emit = 0};

const Material *material__isotropic__build(MemoryArena *p_memory_arena, const Texture *p_texture) {
  Material_Isotropic *p_instance =
      (Material_Isotropic *)memory_arena__allocate(p_memory_arena, sizeof(Material_Isotropic));

  p_instance->p_texture = p_texture;

  return material__build(p_memory_arena, p_instance, p_isotropic_material_interface);
}