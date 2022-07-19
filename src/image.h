#ifndef IMAGE_H
#define IMAGE_H

#include <png.h>
#include "platform.h"

#ifdef _WIN32
int generate_tiles(const wchar_t *in_path, const wchar_t *out_dir);
int row_to_tiles(const wchar_t *out_dir, int rowc, int width, int height, png_byte **src);
int write_file(const wchar_t *out_path, int height, png_byte **src);
#else
int generate_tiles(const char *in_path, const char *out_dir);
int row_to_tiles(const char *out_dir, int rowc, int width, int height, png_byte **src);
int write_file(char *out_path, int width, int height, png_byte **src);
#endif

int get_zoom_levels(int size, int tile_size);
int get_padded_size(int size, int tile_size);
int shrink_pow2(png_byte **dest, int shrink_by, int width, int height, png_byte **src);

#endif /* IMAGE_H */