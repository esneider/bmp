/**
 * @file bmp.h
 *
 * @author Dario Sneidermanis
 */

#ifndef _BMP_H_
#define _BMP_H_

#include <stdint.h>


struct bitmap_info;


struct rgba_color {

    uint8_t a;
	uint8_t r;
    uint8_t g;
    uint8_t b;
};


struct bitmap {

	size_t width;
	size_t height;

	struct rgba_color **bitmap;
	struct bitmap_info *info;
};


/**
 * Creates a color bitmap
 *
 * @param width   nonzero width of the bitmap
 * @param height  nonzero height of the bitmap
 *
 * @return a pointer to a bitmap struct, or NULL if an error ocurred
 */
struct bitmap* new_bmp(size_t width, size_t height);


/**
 * Frees the memory allocated for a bitmap
 *
 * @param bmp  pointer to the bitmap struct
 */
void free_bmp(struct bitmap *bmp);


/**
 * Reads a bmp file
 *
 * @param file_name  name of the bmp file
 *
 * @return a pointer to a bitmap struct, or NULL if an error ocurred
 */
struct bitmap* read_bmp(char *file_name);


/**
 * Writes a 24-bit color bmp file
 *
 * @param bmp        pointer to the bitmap struct
 * @param file_name  name of the bmp file
 *
 * @return 0 if the operation was succesful, or !=0 if an error ocurred
 */
/* int write_bmp(struct bitmap *bmp, char *file_name); */


#endif /* _BMP_H_ */

