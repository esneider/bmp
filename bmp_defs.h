/**
 * @file bmp_defs.h
 *
 * @author Dario Sneidermanis
 */

#ifndef _BMP_DEFS_H_
#define _BMP_DEFS_H_

#include <stdint.h>


/**
 * Color for a bitmap pixel.
 */
struct rgba;


/**
 * Bitmap meta data struct definition.
 */
struct bmp_info {

    struct bmp_header {

        uint16_t padding;
        uint16_t magic_signature;
        uint32_t file_size;
        uint32_t reserved;
        uint32_t bitmap_offset;

    } bmp;

    struct dib_header {

        uint32_t header_size;
        int32_t  bitmap_width;
        int32_t  bitmap_height;
        uint16_t color_planes;
        uint16_t bits_per_pixel;
        uint32_t compression;
        uint32_t bitmap_size;
        int32_t  x_pixels_per_meter;
        int32_t  y_pixels_per_meter;
        uint32_t used_colors;
        uint32_t important_colors;

    } dib;

    struct v3_header {

        uint32_t red_mask;
        uint32_t green_mask;
        uint32_t blue_mask;
        uint32_t alpha_mask;

    } v3;

    struct rgba *colors;
};


/**
 * Bitmap compression modes.
 */
enum bmp_compression {

    BMP_C_RGB,       /* no compression */
    BMP_C_RLE8,      /* 8-bit run-length encoding */
    BMP_C_RLE4,      /* 4-bit run-length encoding */
    BMP_C_BITFIELDS, /* variable bit field encoding */
    BMP_C_JPEG,
    BMP_C_PNG,
    BMP_C_ALPHABITFIELDS, /* variable bit field encoding with alpha channel */
};


#endif /* _BMP_DEFS_H_ */

