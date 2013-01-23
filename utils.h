/**
 * @file utils.h
 *
 * @author Dario Sneidermanis
 */

#ifndef _UTILS_H_
#define _UTILS_H_


typedef unsigned int uint;


/**
 * Allocates memory for a matrix
 *
 * @param width      the width of the matrix
 * @param height     the height of the matrix
 * @param elem_size  the size of each element
 *
 * @return a pointer to the matrix, or NULL if an error ocurred
 */

void** calloc_matrix( uint width, uint height , uint elem_size );


/**
 * Frees the memory allocated for a matrix
 *
 * @param matrix  a pointer to the matrix
 * @param height  the height of the matrix
 */

void free_matrix( void** matrix, uint height );


#endif
