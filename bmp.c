/**
 * @file bmp.c
 *
 * @author Dario Sneidermanis
 */

#include <stdio.h>
#include <stdlib.h>
#include "bmp.h"
#include "bmp_info.h"


static inline int min(int a, int b) {

    return a < b ? a : b;
}


/**
 * Reverse (mirror) the bytes of a 32-bit integer.
 */
static inline void reverse_bytes(uint32_t *n) {

    *n = (*n >> 24) | ((*n >> 8) & 0xFF00) | ((*n & 0xFF00) << 8) | (*n << 24);
}


/**
 * Create a new bitmap.
 *
 * @return a pointer to the struct, or NULL if there was an error.
 */
struct bmp* bmp_new(struct bmp_info *info) {

    struct bmp *bmp;

    if ((bmp = malloc(sizeof(*bmp)))) {

        bmp->info = info;
        bmp->width = info.dib.bitmap_width;
        bmp->height = info.dib.bitmap_height;

        if ((bmp->bmp = malloc(bmp->height * sizeof(*bmp->bmp))))
        if ((bmp->bmp[0] = malloc(bmp->height * bmp->width * sizeof(*bmp->bmp[0])))) {

            for (size_t row = 0; row < bmp->height - 1; row++) {

                bmp->bitmap[row + 1] = bmp->bitmap[row] + bmp->width;
            }
        }
    }

    bmp_free(bmp);
    return NULL;
}


/**
 * Release resources used by the bmp data structure.
 */
void bmp_free(struct bmp *bmp) {

    if (bmp) {

        if (bmp->bmp) {

            free(bmp->bmp[0]);
            free(bmp->bmp);
        }

        bmp_info_free(bmp->info);
        free(bmp);
    }
}


static bool process_image(struct bitmap *bmp, uint8_t *buffer) {

    // TODO: if ((int32_t)bmp->info.dib.bitmap_height < 0) { dar vuelta }
    // TODO: compression
    // TODO: 32-bits BMP could be way faster

    size_t padding = row_bits(bmp->info);

    padding = ((padding + 31) & ~31) / bmp->info->dib.bits_per_pixel - padding / bmp->info->dib.bits_per_pixel;

    for (size_t pos = 0, y = bmp->height; y-- > 0; pos += padding) {

        for (size_t x = 0; x < bmp->width; x++, pos++) {

            switch (bmp->info->dib.bits_per_pixel) {

                case 1:
                    bmp->bitmap[y][x] = bmp->info->colors[(buffer[pos >> 3] >> (7 - (pos & 7))) & 1];
                    break;

                case 4:
                    bmp->bitmap[y][x] = bmp->info->colors[(buffer[pos >> 1] >> (1 - (pos & 1))) & 0xF];
                    break;

                case 8:
                    bmp->bitmap[y][x] = bmp->info->colors[buffer[pos]];
                    break;

                case 16:
                    bmp->bitmap[y][x].b = (buffer[pos] & 31) << 3;
                    bmp->bitmap[y][x].g = ((buffer[pos] >> 2) & ~7) | (buffer[pos + 1] << 6);
                    bmp->bitmap[y][x].r = (buffer[++pos] << 1) & ~7;
                    bmp->bitmap[y][x].a = 0;
                    break;

                case 24:
                    bmp->bitmap[y][x].b = buffer[pos++];
                    bmp->bitmap[y][x].g = buffer[pos++];
                    bmp->bitmap[y][x].r = buffer[pos];
                    bmp->bitmap[y][x].a = 0;
                    break;

                case 32:
                    bmp->bitmap[y][x].b = buffer[pos++];
                    bmp->bitmap[y][x].g = buffer[pos++];
                    bmp->bitmap[y][x].r = buffer[pos++];
                    bmp->bitmap[y][x].a = buffer[pos];
                    break;
            }
        }
    }

    /* Update header */

    bmp->info->dib.bits_per_pixel = 24;
    bmp->info->dib.compression = BMPC_RGB;
    bmp->info->dib.used_colors = 0;
    bmp->info->dib.important_colors = 0;

    set_bitmap_size(bmp->info);
    set_file_size(bmp->info);

    return true;
}



#define SZ_BMPH (sizeof(struct bmp_header) - 2) /* minus unused size */
#define SZ_HEAD (sizeof(struct dib_header) + sizeof(struct v3_header))
#define SZ_RGBA  sizeof(struct rgba_color)


/**
 * Reads a bmp file
 *
 * @param file_name  name of the bmp file
 *
 * @return a pointer to a bitmap struct, or NULL if an error ocurred
 */
struct bitmap* read_bmp(char* file_name) {

    struct bitmap_info *info = malloc(sizeof(*info));

    FILE *file = fopen(file_name, "rb");

    struct bitmap *bmp = NULL;

    uint8_t *buffer = NULL;

    /* Read headers */

    if (file && info)

    if (fread(&info->bmp.magic_signature, SZ_BMPH + 4, 1, file))

    if (fread(&info->dib.bitmap_width, min(info->dib.header_size, SZ_HEAD) - 4, 1, file)) {

        set_bitmap_size(info);

        /* Read color table */

        if ((info->colors = malloc(info->dib.used_colors * SZ_RGBA)))

        if (!fseek(file, info->dib.header_size + SZ_BMPH, SEEK_SET))

        if (fread(info->colors, SZ_RGBA, info->dib.used_colors, file) == info->dib.used_colors)

        /* Read image */

        if ((buffer = malloc(info->dib.bitmap_size)))

        if (!fseek(file, info->bmp.bitmap_offset, SEEK_SET))

        if (fread(buffer, info->dib.bitmap_size, 1, file))

        /* Process image */

        if ((bmp = new_bmp(abs(info->dib.bitmap_width), abs(info->dib.bitmap_height)))) {

            free(bmp->info);
            bmp->info = info;
            info = NULL;

            /* for (size_t i = 0; i < info->dib.used_colors; i++) { */

            /*     reverse_bytes((uint32_t*)info->colors + i); */
            /* } */

            if (process_image(bmp, buffer)) {

                fclose(file);
                free(buffer);

                return bmp;
            }
        }
    }

    if (file) fclose(file);

    free(info);
    free(buffer);
    free_bmp(bmp);

    return NULL;
}


/**
 * Writes a 24-bit color bmp file
 *
 * @param bmp        pointer to the BitMap struct
 * @param file_name  name of the bmp file
 *
 * @return 0 if the operation was succesful, or !=0 if an error ocurred
 */
/* int write_bmp(BitMap* bmp, char* file_name) { */
/*  */
/*  int pos, i, j; */
/*  */
/*  uint8_t* buffer; */
/*  */
/*  FILE* file = fopen(file_name, "wb"); */
/*  */
/*  if (file == NULL) { */
/*      return 1; */
/*  } */
/*  */
/*  if (bmp == NULL || bmp->info == NULL) { */
/*      fclose(file); */
/*      return 1; */
/*  } */
/*  */
/*  buffer = (uint8_t*)calloc(size_bmp(bmp->width, bmp->height), 1); */
/*  */
/*  if (buffer == NULL) { */
/*      fclose(file); */
/*      return 1; */
/*  } */
/*  */
/*  pos = 0; */
/*  write(buffer, pos, i, bmp->info->magic); */
/*  write(buffer, pos, i, size_bmp(bmp->width, bmp->height)); */
/*  write(buffer, pos, i, bmp->info->unused); */
/*  write(buffer, pos, i, bmp->info->offset_data); */
/*  write(buffer, pos, i, bmp->info->header_bytes); */
/*  write(buffer, pos, i, bmp->width); */
/*  write(buffer, pos, i, bmp->height); */
/*  write(buffer, pos, i, bmp->info->color_planes); */
/*  write(buffer, pos, i, bmp->info->color_bpp); */
/*  write(buffer, pos, i, bmp->info->compression); */
/*  write(buffer, pos, i, size_data(bmp->width, bmp->height)); */
/*  write(buffer, pos, i, bmp->info->h_resolution); */
/*  write(buffer, pos, i, bmp->info->v_resolution); */
/*  write(buffer, pos, i, bmp->info->colors_palette); */
/*  write(buffer, pos, i, bmp->info->mean_palette); */
/*  */
/*  for(i=bmp->height-1; i>=0; i--) { */
/*      for(j=0; j<(int)bmp->width; j++) { */
/*          buffer[pos++] = bmp->bitmap[i][j].b; */
/*          buffer[pos++] = bmp->bitmap[i][j].g; */
/*          buffer[pos++] = bmp->bitmap[i][j].r; */
/*      } */
/*      pos += bmp->width%4; */
/*  } */
/*  */
/*  if (fwrite(buffer, sizeof(uint8_t), size_bmp(bmp->width, bmp->height), file) != */
/*                                              size_bmp(bmp->width, bmp->height)) { */
/*  */
/*      fclose(file); */
/*      free(buffer); */
/*      return 1; */
/*  } */
/*  free(buffer); */
/*  */
/*  fclose(file); */
/*  */
/*  return 0; */
/* } */

