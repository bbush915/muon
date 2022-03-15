#include <stdbool.h>

#include "hittable_base.h"
#include "material_base.h"
#include "muon_common.h"
#include "ray.h"
#include "texture_base.h"

typedef struct {
  const Texture *p_texture;
} Material_Diiffuse_Light;

static bool
material__diffuse_light__scatter(const Material_Diiffuse_Light *p_material_diffuse_light,
                                 const Ray *p_ray_in,
                                 const Hit *p_hit,
                                 Ray *p_ray_out,
                                 C3 *p_attenuation) {
  return false;
}

static C3 material__diffuse_light__emit(const Material_Diiffuse_Light *p_material_diffuse_light,
                                        double u,
                                        double v,
                                        P3 position) {
  return texture__value(p_material_diffuse_light->p_texture, u, v, position);
}

static const MaterialInterface *p_diffuse_light_material_interface = &(MaterialInterface){
    .fp_scatter = (bool (*)(
        const void *, const Ray *, const Hit *, Ray *, C3 *))material__diffuse_light__scatter,
    .fp_emit = (C3(*)(const void *, double, double, P3))material__diffuse_light__emit};

const Material *material__diffuse_light__build(MemoryArena *p_memory_arena,
                                               const Texture *p_texture) {
  Material_Diiffuse_Light *p_instance = (Material_Diiffuse_Light *)memory_arena__allocate(
      p_memory_arena, sizeof(Material_Diiffuse_Light));

  p_instance->p_texture = p_texture;

  return material__build(p_memory_arena, p_instance, p_diffuse_light_material_interface);
}
