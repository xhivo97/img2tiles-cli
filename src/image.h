#ifndef IMAGE_H
#define IMAGE_H

#include <png.h>
#include <jpeglib.h>

#include "platform.h"

enum extension { IMAGE_NOT_SUPPORTED, IMAGE_PNG, IMAGE_JPEG };

struct image {
    enum extension ext;
    int tile_size;
    FILE *fp;
    struct {
        png_structp png_ptr;
        png_infop info_ptr;
        png_byte **row_pointers;
        png_uint_32 width;
        png_uint_32 height;
        int color_type;
        int bit_depth;
    };
    int (*init_read) (struct image *img);
    int (*read_row) (void *dest, struct image *img);
    int (*destroy_read) (struct image *img);
};

#ifdef _WIN32
int image_struct_init(struct image *img, const wchar_t *in_path, const wchar_t *out_dir);
int check_extension(const wchar_t *in_path);
int generate_tiles(const wchar_t *in_path, const wchar_t *out_dir);
int row_to_tiles(const wchar_t *out_dir, int rowc, int width, int height, png_byte **src);
int write_file(const wchar_t *out_path, int height, png_byte **src);
#else
int image_struct_init(struct image *img, const char *in_path, const char *out_dir);
int check_extension(const char *in_path);
int generate_tiles(const char *in_path, const char *out_dir);
int row_to_tiles(const char *out_dir, int rowc, int width, int height, png_byte **src);
int write_file(char *out_path, int width, int height, png_byte **src);
#endif

int image_init_read(struct image *img);
int image_struct_destroy(struct image *img);
int image_init_read_png (struct image *img);

int get_zoom_levels(int size, int tile_size);
int get_padded_size(int size, int tile_size);
int shrink_pow2(png_byte **dest, int shrink_by, int width, int height, png_byte **src);

#endif /* IMAGE_H */