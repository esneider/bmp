/**
 * @file bmp.c
 *
 * @author Dario Sneidermanis
 */

#include <stdio.h>
#include <stdlib.h>

#include "bmp.h"


#define size_header()  (uint32_t)default_info.offset_data
#define size_data(width, height) (uint32_t)((height) * ((width)*3 + (width)%4))
#define size_bmp(width, height)  (size_header() + size_data(width, height))

struct BitMapInfo{
    uint16_t magic;
    uint32_t unused;
    uint32_t offset_data;
    uint32_t header_bytes;
    uint16_t color_planes;
    uint16_t color_bpp;
    uint32_t compression;
    int32_t  h_resolution;
    int32_t  v_resolution;
    uint32_t colors_palette;
    uint32_t mean_palette;
};

static const BitMapInfo default_info = {19778,0,54,40,1,24,0,2835,2835,0,0};


/**
 * Returns an empty BitMap struct of the given size
 *
 * @param width   the width of the bmp
 * @param height  the height of the bmp
 *
 * @return a pointer to a BitMap struct, or NULL if an error ocurred
 */

BitMap* new_bmp( uint width, uint height ) {

    BitMap *bmp = (BitMap*)malloc( sizeof(BitMap) );

    if ( bmp == NULL ) {
        return NULL;
    }

    bmp->width = width;
    bmp->height = height;

    bmp->info = (BitMapInfo*)malloc( sizeof(BitMapInfo) );

    if ( bmp->info == NULL ) {
        free_bmp(bmp);
        return NULL;
    }

    *bmp->info = default_info;

    bmp->bitmap = (RGBcolor**)calloc_matrix( width, height, sizeof(RGBcolor) );

    if ( bmp->bitmap == NULL ) {
        free_bmp(bmp);
        return NULL;
    }

    return bmp;
}


/**
 * Frees the memory allocated for a BitMap
 *
 * @param bmp  a pointer to the BitMap struct
 */

void free_bmp( BitMap* bmp ) {

    if ( bmp != NULL ) {
        free( bmp->info );
        free_matrix( (void**)bmp->bitmap, bmp->height );
        free( bmp );
    }
}


#define read( buffer, pos, i, to ) \
            for( to=0, i=sizeof(to)-1; i>=0; i-- ) \
                to |= buffer[pos+i]<<(8*i); \
            pos += sizeof(to)


/**
 * Reads a bmp file
 *
 * @param file_name  the name of the bmp file
 *
 * @return a pointer to a BitMap struct, or NULL if an error ocurred
 */

BitMap* read_bmp( char* file_name ) {

    int pos, i, j;
    uint32_t trash;

    uint8_t* buffer;
    BitMapInfo info;

    uint width, height;

    BitMap* bmp;

    FILE* file = fopen( file_name, "rb" );

    if ( file == NULL ) {
        return NULL;
    }

    buffer = (uint8_t*)malloc( size_header() );

    if ( buffer == NULL ) {
        fclose( file );
        return NULL;
    }

    if ( fread( buffer, sizeof(uint8_t), size_header(), file ) != size_header() ) {

        fclose( file );
        free( buffer );
        return NULL;
    }

    pos = 0;
    read( buffer, pos, i, info.magic );
    read( buffer, pos, i, trash );
    read( buffer, pos, i, info.unused );
    read( buffer, pos, i, info.offset_data );
    read( buffer, pos, i, info.header_bytes );
    read( buffer, pos, i, width );
    read( buffer, pos, i, height );
    read( buffer, pos, i, info.color_planes );
    read( buffer, pos, i, info.color_bpp );
    read( buffer, pos, i, info.compression );
    read( buffer, pos, i, trash );
    read( buffer, pos, i, info.h_resolution );
    read( buffer, pos, i, info.v_resolution );
    read( buffer, pos, i, info.colors_palette );
    read( buffer, pos, i, info.mean_palette );
    free( buffer );

    bmp = new_bmp( width, height );

    if ( bmp == NULL ) {
        fclose( file );
        return NULL;
    }

    *bmp->info = info;

    buffer = (uint8_t*)malloc( size_data(width, height) );

    if ( buffer == NULL ) {
        fclose( file );
        free_bmp( bmp );
        return NULL;
    }

    if ( fread( buffer, sizeof(uint8_t), size_data(width, height), file ) !=
                                                        size_data(width, height) ) {

        fclose( file );
        free( buffer );
        free_bmp( bmp );
        return NULL;
    }

    pos = 0;
    for( i=height-1; i>=0; i-- ) {
        for( j=0; j<(int)width; j++ ) {
            bmp->bitmap[i][j].b = buffer[pos++];
            bmp->bitmap[i][j].g = buffer[pos++];
            bmp->bitmap[i][j].r = buffer[pos++];
        }
        pos += width%4;
    }
    free(buffer);

    fclose( file );

    return bmp;
}


#define write( buffer, pos, i, to ) \
            for( i=sizeof(to)-1; i>=0; i-- ) \
                buffer[pos+i] = (uint8_t)(to>>(8*i)); \
            pos += sizeof(to)


/**
 * Writes a bmp file
 *
 * @param bmp        a pointer to the BitMap struct
 * @param file_name  the name of the bmp file
 *
 * @return 0 if the operation was succesful, or !=0 if an error ocurred
 */

int write_bmp( BitMap* bmp, char* file_name ) {

    int pos, i, j;

    uint8_t* buffer;

    FILE* file = fopen( file_name, "wb" );

    if ( file == NULL ) {
        return 1;
    }

    if ( bmp == NULL || bmp->info == NULL ) {
        fclose( file );
        return 1;
    }

    buffer = (uint8_t*)calloc( size_bmp( bmp->width, bmp->height ), 1 );

    if ( buffer == NULL ) {
        fclose( file );
        return 1;
    }

    pos = 0;
    write( buffer, pos, i, bmp->info->magic );
    write( buffer, pos, i, size_bmp( bmp->width, bmp->height ) );
    write( buffer, pos, i, bmp->info->unused );
    write( buffer, pos, i, bmp->info->offset_data );
    write( buffer, pos, i, bmp->info->header_bytes );
    write( buffer, pos, i, bmp->width );
    write( buffer, pos, i, bmp->height );
    write( buffer, pos, i, bmp->info->color_planes );
    write( buffer, pos, i, bmp->info->color_bpp );
    write( buffer, pos, i, bmp->info->compression );
    write( buffer, pos, i, size_data( bmp->width, bmp->height ) );
    write( buffer, pos, i, bmp->info->h_resolution );
    write( buffer, pos, i, bmp->info->v_resolution );
    write( buffer, pos, i, bmp->info->colors_palette );
    write( buffer, pos, i, bmp->info->mean_palette );

    for( i=bmp->height-1; i>=0; i-- ) {
        for( j=0; j<(int)bmp->width; j++ ) {
            buffer[pos++] = bmp->bitmap[i][j].b;
            buffer[pos++] = bmp->bitmap[i][j].g;
            buffer[pos++] = bmp->bitmap[i][j].r;
        }
        pos += bmp->width%4;
    }

    if ( fwrite( buffer, sizeof(uint8_t), size_bmp(bmp->width, bmp->height), file ) !=
                                                size_bmp(bmp->width, bmp->height) ) {

        fclose( file );
        free( buffer );
        return 1;
    }
    free(buffer);

    fclose( file );

    return 0;
}
