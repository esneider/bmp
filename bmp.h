/**
 * @file bmp.h
 *
 * @author Dario Sneidermanis
 */

#ifndef _BMP_H_
#define _BMP_H_

#include <stdint.h>
#include "utils.h"


typedef struct {
    uint8_t r,g,b;
} RGBcolor;


typedef struct BitMapInfo BitMapInfo;


typedef struct {
    int width;
    int height;
    RGBcolor** bitmap;
    BitMapInfo*  info;
} BitMap;


/**
 * Returns an empty BitMap struct of the given size
 *
 * @param width   the width of the bmp
 * @param height  the height of the bmp
 *
 * @return a pointer to a BitMap struct, or NULL if an error ocurred
 */

BitMap* new_bmp( uint width, uint height );


/**
 * Frees the memory allocated for a BitMap
 *
 * @param bmp  a pointer to the BitMap struct
 */

void free_bmp( BitMap* bmp );


/**
 * Reads a bmp file
 *
 * @param file_name  the name of the bmp file
 *
 * @return a pointer to a BitMap struct, or NULL if an error ocurred
 */

BitMap* read_bmp( char* file_name );


/**
 * Writes a bmp file
 *
 * @param bmp        a pointer to the BitMap struct
 * @param file_name  the name of the bmp file
 *
 * @return 0 if the operation was succesful, or !=0 if an error ocurred
 */

int write_bmp( BitMap* bmp, char* file_name );


#endif
