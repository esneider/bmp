/**
 * @file bmp_def.h
 *
 * @author Dario Sneidermanis
 */

#ifndef _BMP_DEF_H_
#define _BMP_DEF_H_

#include <stdint.h>


struct rgba_color;


/**
 * Types of bitmap compression
 */
enum bitmap_compression {

    BMPC_RGB,       /* no compression */
    BMPC_RLE8,      /* 8-bit run-length encoding */
    BMPC_RLE4,      /* 4-bit run-length encoding */
    BMPC_BITFIELDS, /* variable bit field encoding */
    BMPC_JPEG,
    BMPC_PNG,
    BMPC_ALPHABITFIELDS, /* variable bit field encoding with alpha channel */
};


/**
 * Bitmap metadata
 */
struct bitmap_info {

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

    struct rgba_color *colors;
};


#endif /* _BMP_DEF_H_ */

