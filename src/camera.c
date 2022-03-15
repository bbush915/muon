#include <math.h>

#include "camera.h"
#include "muon_common.h"
#include "ray.h"

const Camera *camera__build(MemoryArena *p_memory_arena,
                            P3 look_from,
                            P3 look_at,
                            V3 view_up,
                            double vertical_field_of_view,
                            double aspect_ratio,
                            double aperture,
                            double focus_distance,
                            double time_start,
                            double time_finish) {
  Camera *p_camera = (Camera *)memory_arena__allocate(p_memory_arena, sizeof(Camera));

  p_camera->position = look_from;

  const double theta = utils__degrees_to_radians(vertical_field_of_view);
  const double h = tan(theta / 2.0);

  p_camera->view_height = 2.0 * h;
  p_camera->view_width = p_camera->view_height * aspect_ratio;

  p_camera->w = v3__unit(v3__sub(look_from, look_at));
  p_camera->u = v3__unit(v3__cross(view_up, p_camera->w));
  p_camera->v = v3__cross(p_camera->w, p_camera->u);

  p_camera->horizontal = v3__mul(p_camera->u, focus_distance * p_camera->view_width);
  p_camera->vertical = v3__mul(p_camera->v, focus_distance * p_camera->view_height);

  p_camera->lower_left_corner =
      v3__sub(v3__sub(v3__sub(p_camera->position, v3__div(p_camera->horizontal, 2.0)),
                      v3__div(p_camera->vertical, 2.0)),
              v3__mul(p_camera->w, focus_distance));

  p_camera->lens_radius = aperture / 2.0;

  p_camera->time_start = time_start;
  p_camera->time_finish = time_finish;

  return p_camera;
}

Ray camera__ray(const Camera *p_camera, double s, double t) {
  // NOTE - Introduce defocus blur.

  const V3 defocus_weights = v3__mul(v3__random_in_disk(1.0), p_camera->lens_radius);
  const V3 defocus_offset =
      v3__add(v3__mul(p_camera->u, defocus_weights.x), v3__mul(p_camera->v, defocus_weights.y));

  // NOTE - Build ray.

  return (Ray){.position = v3__add(p_camera->position, defocus_offset),
               .direction = v3__sub(v3__sub(v3__add(v3__add(p_camera->lower_left_corner,
                                                            v3__mul(p_camera->horizontal, s)),
                                                    v3__mul(p_camera->vertical, t)),
                                            p_camera->position),
                                    defocus_offset),
               .time = utils__random_double_in_range(p_camera->time_start, p_camera->time_finish)};
}