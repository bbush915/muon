#ifndef MATERIAL_BASE_H
#define MATERIAL_BASE_H

#include <stdbool.h>

typedef struct Material_s Material;
typedef struct MaterialInterface_s MaterialInterface;

#include "hittable.h"
#include "muon_common.h"
#include "ray.h"

struct Material_s {
  const void *p_instance;
  const MaterialInterface *p_interface;
};

const Material *material__build(MemoryArena *p_memory_arena,
                                const void *p_instance,
                                const MaterialInterface *p_interface);

struct MaterialInterface_s {
  bool (*const fp_scatter)(const void *p_instance,
                           const Ray *p_ray_in,
                           const Hit *p_hit,
                           Ray *p_ray_out,
                           C3 *p_attenuation);
  C3 (*const fp_emit)(const void *p_instance, double u, double v, P3 position);
};

bool material__scatter(const Material *p_material,
                       const Ray *p_ray_in,
                       const Hit *p_hit,
                       Ray *p_ray_out,
                       C3 *p_attenuation);

C3 material__emit(const Material *p_material, double u, double v, P3 position);

#endif