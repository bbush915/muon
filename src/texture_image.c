#include <assert.h>

#include "muon_common.h"
#include "muon_stb.h"
#include "texture_base.h"
#include "texture_image.h"

static const int DESIRED_CHANNELS = 3;

typedef struct {
  uint8_t *p_image_data;
  int width;
  int height;
  int stride_in_bytes;
} Texture_Image;

static C3
texture__image__value(const Texture_Image *p_texture_image, double u, double v, P3 position) {
  // NOTE - If we failed to load the image, we return cyan to indicate that
  // something has gone wrong.

  if (!p_texture_image->p_image_data) {
    return (C3){0.0, 1.0, 1.0};
  }

  // NOTE - Calculate the offset into the image data in order to grab the
  // correct RGB value.

  u = utils__clamp(u, 0.0, 1.0);

  int i = (int)(u * p_texture_image->width);

  if (i >= p_texture_image->width) {
    i = p_texture_image->width - 1;
  }

  v = 1.0 - utils__clamp(v, 0.0, 1.0);

  int j = (int)(v * p_texture_image->height);

  if (j >= p_texture_image->height) {
    j = p_texture_image->height - 1;
  }

  const uint8_t *p_pixel_data =
      p_texture_image->p_image_data +
      (ptrdiff_t)(j * p_texture_image->stride_in_bytes + i * DESIRED_CHANNELS);

  return v3__mul((C3){p_pixel_data[0], p_pixel_data[1], p_pixel_data[2]}, 1.0 / 255.0);
}

static const TextureInterface *p_image_texture_interface =
    &(TextureInterface){.fp_value = (C3(*)(const void *, double, double, P3))texture__image__value};

const Texture *texture__image__build(MemoryArena *p_memory_arena, const char *p_filename) {
  Texture_Image *p_instance =
      (Texture_Image *)memory_arena__allocate(p_memory_arena, sizeof(Texture_Image));

  // NOTE - Load the image file, and ensure it is in the correct format (RGB).

  int actual_channels;

  p_instance->p_image_data = stbi_load(p_filename, &p_instance->width, &p_instance->height,
                                       &actual_channels, DESIRED_CHANNELS);

  assert(actual_channels == DESIRED_CHANNELS);

  p_instance->stride_in_bytes = p_instance->width * DESIRED_CHANNELS;

  return texture__build(p_memory_arena, p_instance, p_image_texture_interface);
}
