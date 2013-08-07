/**
 * @file bmp_info.h
 *
 * @author Dario Sneidermanis
 */

#ifndef _BMP_INFO_H_
#define _BMP_INFO_H_

#include <stddef.h>
#include "bmp_defs.h"


/**
 * Create a new bmp_info.
 *
 * @return a pointer to the struct, or NULL if there was an error.
 */
struct bmp_info* bmp_info_new(void);


/**
 * Release resources used by the info data structure.
 */
void bmp_info_free(struct bmp_info* info);


/**
 * Set the size of the bitmap in pixels.
 * Default: 0 by 0 pixels.
 */
void bmp_info_set_size(struct bmp_info* info, size_t width, size_t height);


/**
 * Set the type of compression for the bitmap.
 * Default: BMP_C_RGB.
 */
void bmp_info_set_compression(struct bmp_info* info, enum bmp_compression mode);


/**
 * Set the number of bits used per pixel.
 * Default: 24 (8 bits for each color component, RGB, and no alpha channel).
 */
void bmp_info_set_bits(struct bmp_info* info, size_t bits);


#endif /* _BMP_INFO_H_ */

