#include <stdlib.h>
#include <stdint.h>
#include "bmp_info.h"


/**
 * Default values for the bitmap meta data.
 */
static const struct bmp_info default_info = {

    .bmp = {
        .magic_signature = 19778,
        .file_size = 54,
        .reserved = 0,
        .bitmap_offset = 54,
    },

    .dib = {
        .header_size = 40,
        .bitmap_width = 0,
        .bitmap_height = 0,
        .color_planes = 1,
        .bits_per_pixel = 24,
        .compression = BMP_C_RGB,
        .bitmap_size = 0,
        .x_pixels_per_meter = 2835,
        .y_pixels_per_meter = 2835,
        .used_colors = 0,
        .important_colors = 0,
    },

    .v3 = {
        .red_mask = 0,
        .green_mask = 0,
        .blue_mask = 0,
        .alpha_mask = 0,
    },

    .colors = NULL,
};


/**
 * Theoretical row size (in bits).
 */
static inline size_t row_bits(struct bmp_info *info) {

    return info->dib.bits_per_pixel * abs(info->dib.bitmap_width);
}


/**
 * Real row size (in bytes).
 */
static inline size_t row_bytes(struct bmp_info *info) {

    return ((row_bits(info) + 31) >> 3) & ~3;
}


/**
 * Bitmap size (in bytes).
 */
static inline void bitmap_size(struct bmp_info *info) {

    return abs(info->dib.bitmap_height) * row_bytes(info);
}


/**
 * File size (in bytes).
 */
static inline void file_size(struct bmp_info *info) {

    return info->bmp.bitmap_offset + bitmap_size(info);
}


/**
 * Create a new bmp_info.
 *
 * @return a pointer to the struct, or NULL if there was an error.
 */
struct bmp_info* bmp_info_new(void) {

    struct bmp_info *info = malloc(sizeof(*info));

    if (info) {

        *info = default_info;
    }

    return info;
}


/**
 * Release resources used by the info data structure.
 */
void bmp_info_free(struct bmp_info* info) {

    if (info) {

        free(info->colors);
        free(info);
    }
}


/**
 * Set the size of the bitmap in pixels.
 * Default: 0 by 0 pixels.
 */
void bmp_info_set_size(struct bmp_info* info, size_t width, size_t height) {

    if (info) {

        info->dib.bitmap_width = width;
        info->dib.bitmap_height = height;

        info->dib.bitmap_size = bitmap_size(info);
        info->bmp.file_size = set_file_size(info);
    }
}


/**
 * Set the type of compression for the bitmap.
 * Default: BMP_C_RGB.
 */
void bmp_info_set_compression(struct bmp_info* info, enum bmp_compression mode) {

    if (info) {

        info->dib.compression = mode;
    }
}


/**
 * Set the number of bits used per pixel.
 * Default: 24 (8 bits for each color component, RGB, and no alpha channel).
 */
void bmp_info_set_bits(struct bmp_info* info, size_t bits) {

    if (info) {

        info->dib.bits_per_pixel = bits;

        info->dib.bitmap_size = bitmap_size(info);
        info->bmp.file_size = set_file_size(info);
    }
}

