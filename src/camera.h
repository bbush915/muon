#ifndef CAMERA_H
#define CAMERA_H

typedef struct Camera_s Camera;

#include "muon_common.h"
#include "ray.h"

struct Camera_s {
  P3 position;
  double view_width;
  double view_height;
  V3 u;
  V3 v;
  V3 w;
  V3 horizontal;
  V3 vertical;
  P3 lower_left_corner;
  double lens_radius;
  double time_start;
  double time_finish;
};

const Camera *camera__build(MemoryArena *p_memory_arena,
                            P3 look_from,
                            P3 look_at,
                            V3 view_up,
                            double vertical_field_of_view,
                            double aspect_ratio,
                            double aperture,
                            double focus_distance,
                            double time_start,
                            double time_finish);

Ray camera__ray(const Camera *p_camera, double s, double t);

#endif