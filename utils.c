/**
 * @file utils.c
 *
 * @author Dario Sneidermanis
 */

#include <stdlib.h>

#include "utils.h"


/**
 * Allocates memory for a matrix
 *
 * @param width      the width of the matrix
 * @param height     the height of the matrix
 * @param elem_size  the size of each element
 *
 * @return a pointer to the matrix, or NULL if an error ocurred
 */

void** calloc_matrix( uint width, uint height , uint elem_size ){

    uint i;
    void** matrix = malloc( sizeof(void*)*height );

    if ( matrix == NULL ) {
        return NULL;
    }

    for( i=0; i<height; i++ ) {
        matrix[i] = calloc( elem_size, width );

        if ( matrix[i] == NULL ) {
            free_matrix( matrix, i );
            return NULL;
        }
    }

    return matrix;
}


/**
 * Frees the memory allocated for a matrix
 *
 * @param matrix  a pointer to the matrix
 * @param height  the height of the matrix
 */

void free_matrix( void** matrix, uint height ) {

    if ( matrix != NULL ) {
        for( height--; height>0; height-- )
            free( matrix[height] );
        free( matrix[0] );
        free( matrix );
    }
}
