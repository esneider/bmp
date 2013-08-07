/**
 * @file bmp.h
 *
 * @author Dario Sneidermanis
 */

#ifndef _BMP_H_
#define _BMP_H_

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include "bmp_defs.h"


/**
 * Color for a bitmap pixel.
 */
struct rgba {

    uint8_t a;
    uint8_t r;
    uint8_t g;
    uint8_t b;
};


/**
 * Bitmap data structure.
 */
struct bmp {

    struct bmp_info *info;

    size_t width;
    size_t height;

    struct rgba **bmp;
};


/**
 * Create a new bitmap.
 *
 * @return a pointer to the struct, or NULL if there was an error.
 */
struct bmp* bmp_new(struct bmp_info *info);


/**
 * Release resources used by the bmp data structure.
 */
void bmp_free(struct bmp *bmp);


/**
 * Read a bmp file.
 *
 * @return a pointer to the struct, or NULL if there was an error.
 */
struct bmp* bmp_read(const char *file_name);


/**
 * Write a bmp file.
 *
 * @return whether the operation was successful (true) or not (false).
 */
bool bmp_write(struct bmp *bmp, const char *file_name);


#endif /* _BMP_H_ */

