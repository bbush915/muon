#include <math.h>

#include "camera.h"
#include "hittable.h"
#include "material.h"
#include "muon_common.h"
#include "scene.h"
#include "texture.h"

const Scene *scene__final_rt_weekend__build(MemoryArena *p_memory_arena) {
  Scene *p_scene = (Scene *)memory_arena__allocate(p_memory_arena, sizeof(Scene));

  p_scene->aspect_ratio = 16.0 / 9.0; // 3.0 / 2.0;
  p_scene->height = 225; // 800;
  p_scene->width = (int)(p_scene->height * p_scene->aspect_ratio);
  p_scene->samples_per_pixel = 100; // 500;
  p_scene->max_depth = 50;
  p_scene->background_color = (C3){0.7, 0.8, 1.0};

  // NOTE - Camera

  const P3 look_from = (P3){13.0, 2.0, 3.0};
  const P3 look_at = (P3){0.0, 0.0, 0.0};
  const V3 view_up = (V3){0.0, 1.0, 0.0};
  const double vertical_field_of_view = 20.0;
  const double aperture = 0.1;
  const double focus_length = 10.0;
  const double time_start = 0.0;
  const double time_finish = 1.0;

  const Camera *p_camera =
      camera__build(p_memory_arena, look_from, look_at, view_up, vertical_field_of_view,
                    p_scene->aspect_ratio, aperture, focus_length, time_start, time_finish);

  p_scene->p_camera = p_camera;

  // NOTE - Hittables

  const Texture *p_ground_texture = texture__solid__build(p_memory_arena, (C3){0.5, 0.5, 0.5});
  const Material *p_ground_material = material__lambertian__build(p_memory_arena, p_ground_texture);
  const Hittable *p_ground_sphere =
      hittable__sphere__build(p_memory_arena, (P3){0.0, -1000.0, 0.0}, 1000.0, p_ground_material);

  const Material *p_dialectric_material = material__dialectric__build(p_memory_arena, 1.5);
  const Hittable *p_dialectric_sphere =
      hittable__sphere__build(p_memory_arena, (P3){0.0, 1.0, 0.0}, 1.0, p_dialectric_material);

  const Texture *p_lambertian_texture = texture__solid__build(p_memory_arena, (C3){0.4, 0.2, 0.1});
  const Material *p_lambertian_material =
      material__lambertian__build(p_memory_arena, p_lambertian_texture);
  const Hittable *p_lambertian_sphere =
      hittable__sphere__build(p_memory_arena, (P3){-4.0, 1.0, 0.0}, 1.0, p_lambertian_material);

  const Material *p_metal_material =
      material__metal__build(p_memory_arena, (C3){0.7, 0.6, 0.5}, 0.0);
  const Hittable *p_metal_sphere =
      hittable__sphere__build(p_memory_arena, (P3){4.0, 1.0, 0.0}, 1.0, p_metal_material);

  DynamicArray *p_random_spheres = dynamic_array__build(p_memory_arena, sizeof(Hittable *), 500);

  for (int i = -11; i < 11; i++) {
    for (int j = -11; j < 11; j++) {
      const P3 position = {i + 0.9 * utils__random_double(), 0.2, j + 0.9 * utils__random_double()};

      if (v3__len(v3__sub(position, (P3){4.0, 0.2, 0.0})) > 0.9) {
        const double value = utils__random_double();

        if (value < 0.8) {
          const C3 albedo = v3__hadamard(v3__random(), v3__random());

          const Texture *p_texture = texture__solid__build(p_memory_arena, albedo);
          const Material *p_material = material__lambertian__build(p_memory_arena, p_texture);
          const Hittable *p_sphere =
              hittable__sphere__build(p_memory_arena, position, 0.2, p_material);

          dynamic_array__push(p_random_spheres, (void *)&p_sphere);
        } else if (value < 0.95) {
          const C3 albedo = v3__random_in_range(0.5, 1.0);
          const double fuzzing_factor = utils__random_double_in_range(0.0, 0.5);

          const Material *p_material =
              material__metal__build(p_memory_arena, albedo, fuzzing_factor);
          const Hittable *p_sphere =
              hittable__sphere__build(p_memory_arena, position, 0.2, p_material);

          dynamic_array__push(p_random_spheres, (void *)&p_sphere);
        } else {
          const double refractive_index = 1.5;

          const Material *p_material =
              material__dialectric__build(p_memory_arena, refractive_index);
          const Hittable *p_sphere =
              hittable__sphere__build(p_memory_arena, position, 0.2, p_material);

          dynamic_array__push(p_random_spheres, (void *)&p_sphere);
        }
      }
    }
  }

  const int hittable_count = 4 + p_random_spheres->item_count;
  const Hittable **pp_hittables = (const Hittable **)memory_arena__allocate(
      p_memory_arena, (size_t)(hittable_count * sizeof(Hittable *)));

  pp_hittables[0] = p_ground_sphere;
  pp_hittables[1] = p_dialectric_sphere;
  pp_hittables[2] = p_lambertian_sphere;
  pp_hittables[3] = p_metal_sphere;

  for (int i = 0; i < p_random_spheres->item_count; i++) {
    pp_hittables[4 + i] = *(Hittable **)dynamic_array__at(p_random_spheres, i);
  }

  const Hittable *p_hittable_bvh = hittable__bounding_volume_hierarchy__build(
      p_memory_arena, pp_hittables, hittable_count, 0, hittable_count, 0.0, 1.0);

  p_scene->p_hittables = p_hittable_bvh;

  return p_scene;
}

const Scene *scene__cornell_box__build(MemoryArena *p_memory_arena) {
  Scene *p_scene = (Scene *)memory_arena__allocate(p_memory_arena, sizeof(Scene));

  p_scene->aspect_ratio = 1.0;
  p_scene->height = 600;
  p_scene->width = (int)(p_scene->height * p_scene->aspect_ratio);
  p_scene->samples_per_pixel = 200;
  p_scene->max_depth = 50;
  p_scene->background_color = (C3){0.0, 0.0, 0.0};

  // NOTE - Camera

  const P3 look_from = (P3){278.0, 278.0, -800.0};
  const P3 look_at = (P3){278.0, 278.0, 0.0};
  const V3 view_up = (V3){0.0, 1.0, 0.0};
  const double vertical_field_of_view = 40.0;
  const double aperture = 0.0;
  const double focus_length = 10.0;
  const double time_start = 0.0;
  const double time_finish = 1.0;

  const Camera *p_camera =
      camera__build(p_memory_arena, look_from, look_at, view_up, vertical_field_of_view,
                    p_scene->aspect_ratio, aperture, focus_length, time_start, time_finish);

  p_scene->p_camera = p_camera;

  // NOTE - Hittables

  const Texture *p_red_texture = texture__solid__build(p_memory_arena, ((C3){0.65, 0.05, 0.05}));
  const Material *p_red_material = material__lambertian__build(p_memory_arena, p_red_texture);

  const Hittable *p_red_rectangle =
      hittable__yz_rectangle__build(p_memory_arena, 0.0, 555.0, 0.0, 555.0, 0.0, p_red_material);

  const Texture *p_white_texture = texture__solid__build(p_memory_arena, (C3){0.73, 0.73, 0.73});
  const Material *p_white_material = material__lambertian__build(p_memory_arena, p_white_texture);

  const Hittable *p_white_1_rectangle =
      hittable__xz_rectangle__build(p_memory_arena, 0.0, 555.0, 0.0, 555.0, 0.0, p_white_material);

  const Hittable *p_white_2_rectangle = hittable__xz_rectangle__build(
      p_memory_arena, 0.0, 555.0, 0.0, 555.0, 555.0, p_white_material);

  const Hittable *p_white_3_rectangle = hittable__xy_rectangle__build(
      p_memory_arena, 0.0, 555.0, 0.0, 555.0, 555.0, p_white_material);

  const Texture *p_green_texture = texture__solid__build(p_memory_arena, (C3){0.12, 0.45, 0.15});
  const Material *p_green_material = material__lambertian__build(p_memory_arena, p_green_texture);

  const Hittable *p_green_rectangle = hittable__yz_rectangle__build(p_memory_arena, 0.0, 555.0, 0.0,
                                                                    555.0, 555.0, p_green_material);

  const Texture *p_light_texture = texture__solid__build(p_memory_arena, (C3){7.0, 7.0, 7.0});
  const Material *p_light_material =
      material__diffuse_light__build(p_memory_arena, p_light_texture);

  const Hittable *p_light_rectangle = hittable__xz_rectangle__build(
      p_memory_arena, 113.0, 443.0, 127.0, 432.0, 554.0, p_light_material);

  const Hittable *p_box_1 = hittable__box__build(p_memory_arena, (P3){0.0, 0.0, 0.0},
                                                 (P3){165.0, 330.0, 165.0}, p_white_material);

  const Hittable *p_rotated_box_1 = hittable__y_rotation__build(p_memory_arena, p_box_1, 15.0);

  const Hittable *p_translated_rotated_box_1 =
      hittable__translation__build(p_memory_arena, p_rotated_box_1, (V3){265.0, 0.0, 295.0});

  const Texture *p_box_1_texture = texture__solid__build(p_memory_arena, (C3){0.0, 0.0, 0.0});

  const Material *p_box_1_material = material__isotropic__build(p_memory_arena, p_box_1_texture);

  const Hittable *p_smokey_translated_rotated_box_1 = hittable__constant_medium__build(
      p_memory_arena, p_translated_rotated_box_1, 0.01, p_box_1_material);

  const Hittable *p_box_2 = hittable__box__build(p_memory_arena, (P3){0.0, 0.0, 0.0},
                                                 (P3){165.0, 165.0, 165.0}, p_white_material);

  const Hittable *p_rotated_box_2 = hittable__y_rotation__build(p_memory_arena, p_box_2, -18.0);

  const Hittable *p_translated_rotated_box_2 =
      hittable__translation__build(p_memory_arena, p_rotated_box_2, (V3){130.0, 0.0, 65.0});

  const Texture *p_box_2_texture = texture__solid__build(p_memory_arena, (C3){1.0, 1.0, 1.0});

  const Material *p_box_2_material = material__isotropic__build(p_memory_arena, p_box_2_texture);

  const Hittable *p_smokey_translated_rotated_box_2 = hittable__constant_medium__build(
      p_memory_arena, p_translated_rotated_box_2, 0.01, p_box_2_material);

  const int hittable_count = 8;
  const Hittable **pp_hittables = (const Hittable **)memory_arena__allocate(
      p_memory_arena, hittable_count * sizeof(Hittable *));

  pp_hittables[0] = p_red_rectangle;
  pp_hittables[1] = p_green_rectangle;
  pp_hittables[2] = p_white_1_rectangle;
  pp_hittables[3] = p_white_2_rectangle;
  pp_hittables[4] = p_white_3_rectangle;
  pp_hittables[5] = p_light_rectangle;
  pp_hittables[6] = p_smokey_translated_rotated_box_1;
  pp_hittables[7] = p_smokey_translated_rotated_box_2;

  const Hittable *p_hittable_bvh = hittable__bounding_volume_hierarchy__build(
      p_memory_arena, pp_hittables, hittable_count, 0, hittable_count, 0.0, 1.0);

  p_scene->p_hittables = p_hittable_bvh;

  return p_scene;
}

const Scene *scene__final_rt_next_week__build(MemoryArena *p_memory_arena) {
  Scene *p_scene = (Scene *)memory_arena__allocate(p_memory_arena, sizeof(Scene));

  p_scene->aspect_ratio = 1.0;
  p_scene->height = 800;
  p_scene->width = (int)(p_scene->height * p_scene->aspect_ratio);
  p_scene->samples_per_pixel = 100; // 10000;
  p_scene->max_depth = 50;
  p_scene->background_color = (C3){0.0, 0.0, 0.0};

  // NOTE - Camera

  const P3 look_from = (P3){478.0, 278.0, -600.0};
  const P3 look_at = (P3){278.0, 278.0, 0.0};
  const V3 view_up = (V3){0.0, 1.0, 0.0};
  const double vertical_field_of_view = 40.0;
  const double aperture = 0.0;
  const double focus_length = 10.0;
  const double time_start = 0.0;
  const double time_finish = 1.0;

  const Camera *p_camera =
      camera__build(p_memory_arena, look_from, look_at, view_up, vertical_field_of_view,
                    p_scene->aspect_ratio, aperture, focus_length, time_start, time_finish);

  p_scene->p_camera = p_camera;

  // NOTE - Hittables

  const Texture *p_light_texture = texture__solid__build(p_memory_arena, (C3){7.0, 7.0, 7.0});
  const Material *p_light_material =
      material__diffuse_light__build(p_memory_arena, p_light_texture);

  const Hittable *p_light_rectangle = hittable__xz_rectangle__build(
      p_memory_arena, 123.0, 423.0, 147.0, 412.0, 554.0, p_light_material);

  const Texture *p_ground_texture = texture__solid__build(p_memory_arena, (C3){0.48, 0.83, 0.53});
  const Material *p_ground_material = material__lambertian__build(p_memory_arena, p_ground_texture);

  const P3 moving_center_1 = (P3){400.0, 400.0, 200.0};
  const P3 moving_center_2 = v3__add(moving_center_1, (V3){30.0, 0.0, 0.0});

  const Texture *p_moving_texture = texture__solid__build(p_memory_arena, (C3){0.7, 0.3, 0.1});
  const Material *p_moving_material = material__lambertian__build(p_memory_arena, p_moving_texture);

  const Hittable *p_moving_sphere = hittable__moving_sphere__build(
      p_memory_arena, moving_center_1, moving_center_2, 0.0, 1.0, 50.0, p_moving_material);

  const Material *p_dialectric_material = material__dialectric__build(p_memory_arena, 1.5);
  const Hittable *p_dialectric_sphere = hittable__sphere__build(
      p_memory_arena, (P3){260.0, 150.0, 45.0}, 50.0, p_dialectric_material);

  const Material *p_metal_material =
      material__metal__build(p_memory_arena, (C3){0.8, 0.8, 0.9}, 1.0);
  const Hittable *p_metal_sphere =
      hittable__sphere__build(p_memory_arena, (P3){0.0, 150.0, 145.0}, 50.0, p_metal_material);

  const Texture *p_earth_texture = texture__image__build(p_memory_arena, "../assets/earth.jpeg");
  const Material *p_earth_material = material__lambertian__build(p_memory_arena, p_earth_texture);

  const Hittable *p_earth_sphere =
      hittable__sphere__build(p_memory_arena, (P3){400.0, 200.0, 400.0}, 100.0, p_earth_material);

  const Texture *p_noise_texture = texture__noise__build(p_memory_arena, 0.1);
  const Material *p_noise_material = material__lambertian__build(p_memory_arena, p_noise_texture);

  const Hittable *p_noise_sphere =
      hittable__sphere__build(p_memory_arena, (P3){220.0, 280.0, 300.0}, 80.0, p_noise_material);

  const Material *p_subsurface_outer_material = material__dialectric__build(p_memory_arena, 1.5);
  const Hittable *p_subsurface_outer_sphere = hittable__sphere__build(
      p_memory_arena, (P3){360.0, 150.0, 145.0}, 70.0, p_subsurface_outer_material);

  const Texture *p_subsurface_inner_texture =
      texture__solid__build(p_memory_arena, (C3){0.2, 0.4, 0.9});
  const Material *p_subsurface_inner_material =
      material__lambertian__build(p_memory_arena, p_subsurface_inner_texture);

  const Hittable *p_subsurface_inner_medium = hittable__constant_medium__build(
      p_memory_arena, p_subsurface_outer_sphere, 0.2, p_subsurface_inner_material);

  const Material *p_mist_boundary_material = material__dialectric__build(p_memory_arena, 1.5);
  const Hittable *p_mist_boundary_sphere = hittable__sphere__build(
      p_memory_arena, (P3){0.0, 0.0, 0.0}, 5000.0, p_mist_boundary_material);

  const Texture *p_mist_texture = texture__solid__build(p_memory_arena, (C3){1.0, 1.0, 1.0});
  const Material *p_mist_material = material__lambertian__build(p_memory_arena, p_mist_texture);
  const Hittable *p_mist_medium = hittable__constant_medium__build(
      p_memory_arena, p_mist_boundary_sphere, 0.0001, p_mist_material);

  const int random_sphere_count = 1000;
  const Hittable **pp_random_spheres = (const Hittable **)memory_arena__allocate(
      p_memory_arena, random_sphere_count * sizeof(Hittable *));

  const Texture *p_random_sphere_texture =
      texture__solid__build(p_memory_arena, (C3){0.73, 0.73, 0.73});
  const Material *p_random_sphere_material =
      material__lambertian__build(p_memory_arena, p_random_sphere_texture);

  for (int i = 0; i < 1000; i++) {
    const Hittable *p_random_sphere = hittable__sphere__build(
        p_memory_arena, v3__random_in_range(0.0, 165.0), 10.0, p_random_sphere_material);

    pp_random_spheres[i] = p_random_sphere;
  }

  const Hittable *p_random_spheres_bvh = hittable__bounding_volume_hierarchy__build(
      p_memory_arena, pp_random_spheres, random_sphere_count, 0, random_sphere_count, 0.0, 1.0);

  const Hittable *p_random_spheres_rotation =
      hittable__y_rotation__build(p_memory_arena, p_random_spheres_bvh, 15.0);

  const Hittable *p_random_spheres_translation = hittable__translation__build(
      p_memory_arena, p_random_spheres_rotation, (V3){-100.0, 270.0, 395.0});

  DynamicArray *p_random_boxes = dynamic_array__build(p_memory_arena, sizeof(Hittable *), 400);

  const double step = 100.0;

  for (int i = 0; i < 20; i++) {
    for (int j = 0; j < 20; j++) {
      const double x0 = -1000.0 + i * step;
      const double x1 = x0 + step;

      const double y0 = 0.0;
      const double y1 = utils__random_double_in_range(1.0, 76.0);

      const double z0 = -1000.0 + j * step;
      const double z1 = z0 + step;

      const Hittable *p_random_box = hittable__box__build(p_memory_arena, (P3){x0, y0, z0},
                                                          (P3){x1, y1, z1}, p_ground_material);

      dynamic_array__push(p_random_boxes, (void *)&p_random_box);
    }
  }

  const int hittable_count = 410;
  const Hittable **pp_hittables = (const Hittable **)memory_arena__allocate(
      p_memory_arena, hittable_count * sizeof(Hittable *));

  pp_hittables[0] = p_light_rectangle;
  pp_hittables[1] = p_moving_sphere;
  pp_hittables[2] = p_dialectric_sphere;
  pp_hittables[3] = p_metal_sphere;
  pp_hittables[4] = p_earth_sphere;
  pp_hittables[5] = p_noise_sphere;
  pp_hittables[6] = p_mist_medium;
  pp_hittables[7] = p_subsurface_inner_medium;
  pp_hittables[8] = p_subsurface_outer_sphere;
  pp_hittables[9] = p_random_spheres_translation;

  for (int i = 0; i < p_random_boxes->item_count; i++) {
    pp_hittables[10 + i] = *(Hittable **)dynamic_array__at(p_random_boxes, i);
  }

  const Hittable *p_hittable_bvh = hittable__bounding_volume_hierarchy__build(
      p_memory_arena, pp_hittables, hittable_count, 0, hittable_count, 0.0, 1.0);

  p_scene->p_hittables = p_hittable_bvh;

  return p_scene;
}