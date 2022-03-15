#include <math.h>
#include <pthread.h>
#include <stdatomic.h>
#include <time.h>
#include <unistd.h>

#include "camera.h"
#include "hittable.h"
#include "muon_common.h"
#include "muon_stb.h"
#include "ray.h"
#include "scene.h"

// NOTE - Since each pixel can be determined independently, the rendering
// process is "embarassingly parallelizable". We can use the value below to
// configure how many render threads to spawn.

static const int DEGREE_OF_PARALLELISM = 4;

// NOTE - How many pixels (per thread) to calculate before updating the
// progress.

static const int PROGRESS_STEP = 10000;

typedef struct {
  const Scene *p_scene;
  int chunk_size;
  int chunk_offset;
  atomic_size_t *p_counter;
  uint8_t *p_image_data;
} RenderContext;

typedef struct {
  atomic_size_t *p_counter;
  int image_size;
} ProgressContext;

static void *print_progress(void *p_argument) {
  ProgressContext *p_progress_context = (ProgressContext *)p_argument;

  while (1) {
    const double percent_complete =
        100.0 * (double)atomic_load(p_progress_context->p_counter) / p_progress_context->image_size;

    fprintf(stdout, "\rProgress: %.2f%%", percent_complete);
    fflush(stdout);

    if (percent_complete >= 100.0) {
      break;
    }

    sleep(5);
  }

  return EXIT_SUCCESS;
}

static C3 trace_ray(const Scene *p_scene, const Ray *p_ray, int depth) {
  if (depth <= 0) {
    return (C3){0.0, 0.0, 0.0};
  }

  Hit hit;

  if (!hittable__hit(p_scene->p_hittables, p_ray, 0.001, INFINITY, &hit)) {
    return p_scene->background_color;
  }

  const C3 emitted_color = material__emit(hit.p_material, hit.u, hit.v, hit.position);

  Ray scattered;
  C3 attenuation;

  if (!material__scatter(hit.p_material, p_ray, &hit, &scattered, &attenuation)) {
    return emitted_color;
  }

  const C3 scattered_color =
      v3__add(emitted_color, v3__hadamard(attenuation, trace_ray(p_scene, &scattered, depth - 1)));

  return scattered_color;
}

static void *render_scene_chunk(void *p_argument) {
  RenderContext *p_render_context = (RenderContext *)p_argument;

  const Scene *p_scene = p_render_context->p_scene;

  const int scene_width = p_scene->width;
  const int scene_height = p_scene->height;

  for (int m = 0; m < p_render_context->chunk_size; m++) {
    const int i = (p_render_context->chunk_offset + m) / scene_width;
    const int j = (p_render_context->chunk_offset + m) % scene_width;

    C3 pixel_color = {};

    for (int n = 0; n < p_scene->samples_per_pixel; n++) {
      const double u = (j + utils__random_double()) / (scene_width - 1);
      const double v = 1.0 - (i + utils__random_double()) / (scene_height - 1);

      const Ray ray = camera__ray(p_scene->p_camera, u, v);
      const C3 sample_color = trace_ray(p_scene, &ray, p_scene->max_depth);

      pixel_color = v3__add(pixel_color, sample_color);
    }

    pixel_color.x = sqrt(pixel_color.x / p_scene->samples_per_pixel);
    pixel_color.y = sqrt(pixel_color.y / p_scene->samples_per_pixel);
    pixel_color.z = sqrt(pixel_color.z / p_scene->samples_per_pixel);

    p_render_context->p_image_data[3 * (p_render_context->chunk_offset + m) + 0] =
        (uint8_t)(256.0 * utils__clamp(pixel_color.x, 0.0, 0.999));
    p_render_context->p_image_data[3 * (p_render_context->chunk_offset + m) + 1] =
        (uint8_t)(256.0 * utils__clamp(pixel_color.y, 0.0, 0.999));
    p_render_context->p_image_data[3 * (p_render_context->chunk_offset + m) + 2] =
        (uint8_t)(256.0 * utils__clamp(pixel_color.z, 0.0, 0.999));

    if (m % PROGRESS_STEP == 0) {
      atomic_fetch_add(p_render_context->p_counter, PROGRESS_STEP);
    }
  }

  atomic_fetch_add(p_render_context->p_counter, p_render_context->chunk_size % PROGRESS_STEP);

  return EXIT_SUCCESS;
}

static const uint8_t *render_scene(const Scene *p_scene) {
  MemoryArena *p_memory_arena = memory_arena__build(MEGABYTES(5));

  const int scene_width = p_scene->width;
  const int scene_height = p_scene->height;

  uint8_t *p_image_data = (uint8_t *)memory_arena__allocate(
      p_memory_arena, (size_t)3 * scene_width * scene_height * sizeof(uint8_t));

  atomic_size_t p_counter = 0;

  pthread_t threads[DEGREE_OF_PARALLELISM + 1];

  // NOTE - Create thread to continuously print progress.

  ProgressContext progress_context = {.image_size = scene_width * scene_height,
                                      .p_counter = &p_counter};

  pthread_create(&threads[0], NULL, print_progress, (void *)&progress_context);

  // NOTE - Create multiple threads to render a chunk of the final scene.

  RenderContext render_contexts[DEGREE_OF_PARALLELISM];

  const int base_chunk_size = scene_width * scene_height / DEGREE_OF_PARALLELISM;
  const int last_chunk_size =
      scene_width * scene_height - (DEGREE_OF_PARALLELISM - 1) * base_chunk_size;

  for (int i = 0; i < DEGREE_OF_PARALLELISM; i++) {
    const int chunk_size = i == DEGREE_OF_PARALLELISM - 1 ? last_chunk_size : base_chunk_size;

    render_contexts[i] = (RenderContext){.p_scene = p_scene,
                                         .chunk_size = chunk_size,
                                         .chunk_offset = i * chunk_size,
                                         .p_counter = &p_counter,
                                         .p_image_data = p_image_data};

    pthread_create(&threads[i + 1], NULL, render_scene_chunk, (void *)&render_contexts[i]);
  }

  // NOTE - Wait for all threads to complete.

  for (int i = 0; i < DEGREE_OF_PARALLELISM + 1; i++) {
    pthread_join(threads[i], NULL);
  }

  memory_arena__destroy(p_memory_arena);

  return p_image_data;
}

static void export_scene(int width, int height, const uint8_t *p_image_data) {
  stbi_write_png("muon_out.png", width, height, 3, p_image_data, 3 * width);
}

int main() {
  MemoryArena *p_memory_arena = memory_arena__build(MEGABYTES(5));

  const Scene *p_scene = scene__final_rt_next_week__build(p_memory_arena);

  fprintf(stdout, "Rendering scene...\n");

  time_t start_time = time(NULL);

  const uint8_t *p_image_data = render_scene(p_scene);
  export_scene(p_scene->width, p_scene->height, p_image_data);

  fprintf(stdout, "\nDone! (%lds)\n", time(NULL) - start_time);

  memory_arena__destroy(p_memory_arena);

  return EXIT_SUCCESS;
}
