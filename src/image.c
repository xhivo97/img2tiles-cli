#include <assert.h>
#include <stdlib.h>
#include "image.h"

#ifdef _WIN32
static const wchar_t *valid_extensions[] = {L".png", L".PNG", L".jpg", L".JPG", L".jpeg", L".JPEG"};
#else
static const char *valid_extensions[] = {".png", ".PNG", ".jpg", ".JPG", ".jpeg", ".JPEG"};
#endif

#ifdef _WIN32
int image_struct_init(struct image *img, const wchar_t *in_path, const wchar_t *out_dir) {
#else
int image_struct_init(struct image *img, const char *in_path, const char *out_dir) {
    img->fp = FOPEN(in_path, "rb");
    if (!img->fp) {
        print_error("could not open: %s\n", getbase(in_path));
        return EXIT_FAILURE;
    }

    img->ext = check_extension(in_path);
    if (img->ext == IMAGE_NOT_SUPPORTED) {
        print_error("file %s is not supported\n", getbase(in_path));
        fclose(img->fp);
        return EXIT_FAILURE;
    }

    if (make_directory(out_dir) != 0) {
        print_error("could not create directory: %s\n", out_dir);
        fclose(img->fp);
        return EXIT_FAILURE;
    }

    img->tile_size = 256;

    if (img->ext == IMAGE_PNG) {
        img->init_read = image_init_read_png;
        img->read_rows = image_read_rows_png;
    }

    return EXIT_SUCCESS;
}
#endif

int image_struct_destroy(struct image *img) {
    fclose(img->fp);
    if (img->png_ptr != NULL) {
        if (setjmp(png_jmpbuf(img->png_ptr)))
            return EXIT_FAILURE;
        if (img->info_ptr != NULL)
            png_destroy_read_struct(&img->png_ptr, &img->info_ptr, NULL);
        else
            png_destroy_read_struct(&img->png_ptr, NULL, NULL);
    }

    return EXIT_SUCCESS;
}

int image_init_read(struct image *img) {
    if (img->init_read(img) == EXIT_FAILURE)
        return EXIT_FAILURE;
    
    image_set_padding(img);
    return EXIT_SUCCESS;
}

int image_read_rows_png(int nr_rows, struct image *img) {
    assert(img->png_ptr != NULL && img->info_ptr != NULL);
    if (setjmp(png_jmpbuf(img->png_ptr)))
        return EXIT_FAILURE;
    png_read_rows(img->png_ptr, img->row_pointers, NULL, nr_rows);

    img->row_count = nr_rows;
    return EXIT_SUCCESS;
}

int image_read_rows(int nr_rows, struct image *img) {
    return img->read_rows(nr_rows, img);
}

void image_set_padding(struct image *img) {
    img->pad_size = get_padded_size(img->width, img->height, img->tile_size);
    img->top_pad = (img->pad_size - img->height) / 2;
    img->left_pad = (img->pad_size - img->width) / 2;
    img->levels = get_zoom_levels(img->pad_size, img->tile_size);

    img->ofs_x = img->left_pad * 4;
    img->ofs_y = img->top_pad;
}

int image_init_read_png (struct image *img) {
    img->png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING,
        NULL, NULL, NULL);
    if (!img->png_ptr)
        return EXIT_FAILURE;

    img->info_ptr = png_create_info_struct(img->png_ptr);
    if(!img->info_ptr)
        return EXIT_FAILURE;

    if (setjmp(png_jmpbuf(img->png_ptr)))
        return EXIT_FAILURE;

    png_init_io(img->png_ptr, img->fp);
    png_read_info(img->png_ptr, img->info_ptr);

    png_get_IHDR(img->png_ptr, img->info_ptr, &img->width, &img->height,
            &img->bit_depth, &img->color_type, NULL, NULL, NULL);

    if (img->color_type == PNG_COLOR_TYPE_PALETTE)
        png_set_palette_to_rgb(img->png_ptr);

    if (png_get_valid(img->png_ptr, img->info_ptr, PNG_INFO_tRNS))
        png_set_tRNS_to_alpha(img->png_ptr);

    if (img->color_type == PNG_COLOR_TYPE_GRAY &&
        img->bit_depth < 8) png_set_expand_gray_1_2_4_to_8(img->png_ptr);

    if (img->bit_depth == 16)
#if PNG_LIBPNG_VER >= 10504
        png_set_scale_16(img->png_ptr);
#else
        png_set_strip_16(img->png_ptr);
#endif

    if (img->color_type == PNG_COLOR_TYPE_RGB ||
            img->color_type == PNG_COLOR_TYPE_GRAY)
        png_set_add_alpha(img->png_ptr, 0xFF, PNG_FILLER_AFTER);

    png_read_update_info(img->png_ptr, img->info_ptr);

    img->row_pointers = malloc(sizeof(png_bytep) * img->height);
    if (img->row_pointers == NULL)
        return EXIT_FAILURE;
    for (int i = 0; i < img->tile_size; i++) {
        img->row_pointers[i] = malloc(sizeof(png_byte) * img->width * 4);
        if (img->row_pointers[i] == NULL)
            return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

#ifdef _WIN32
int generate_tiles(const wchar_t *in_path, const wchar_t *out_dir) {
#else
int generate_tiles(const char *in_path, const char *out_dir) {
#endif
    FILE *fp = FOPEN(in_path, "rb");
    if (!fp) {
        print_error("could not open: %s\n", getbase(in_path));
        return EXIT_FAILURE;
    }

    if (check_extension(in_path) == IMAGE_NOT_SUPPORTED) {
        print_error("file %s is not supported\n", getbase(in_path));
        fclose(fp);
        return EXIT_FAILURE;
    }

    if (make_directory(out_dir) != 0) {
        print_error("could not create directory: %s\n", out_dir);
        fclose(fp);
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

#ifdef _WIN32
int check_extension(const wchar_t *in_path) {
    if (STRLEN(in_path) < 5)
        return IMAGE_NOT_SUPPORTED;
    const wchar_t *ext = in_path + STRLEN(in_path)-5;
#else
int check_extension(const char *in_path) {
    if (STRLEN(in_path) < 5)
        return IMAGE_NOT_SUPPORTED;
    const char *ext = in_path + STRLEN(in_path)-5;
#endif
    int size = sizeof(valid_extensions) / sizeof(valid_extensions[0]);

    if (ext[0] == '.') {
        for (int i = 0; i < size; i++) {
            if (STRCMP(ext, valid_extensions[i]) == 0)
                return IMAGE_JPEG;
        }
    } else {
        ext++;
        for (int i = 0; i < size; i++) {
            if (STRCMP(ext, valid_extensions[i]) == 0) {
                if (ext[1] == 'p' || ext[1] == 'P')
                    return IMAGE_PNG;

                return IMAGE_JPEG;
            }
        }
    }
    
    return IMAGE_NOT_SUPPORTED;
}

int get_padded_size(int width, int  height, int tile_size) {
    int ret = height < width ? width : height;
	if ((tile_size & (tile_size-1)) != 0)
		return -1;
	if (ret < tile_size)
		return tile_size;
	ret--;
	ret |= ret >> 1;
	ret |= ret >> 2;
	ret |= ret >> 4;
	ret |= ret >> 8;
	ret |= ret >> 16;
	ret++;
	if ((ret % tile_size) == 0)
		return ret;
	ret |= ret >> 1;
	ret |= ret >> 2;
	ret |= ret >> 4;
	ret |= ret >> 8;
	ret |= ret >> 16;
	ret++;
	return ret;
}

int get_zoom_levels(int size, int tile_size) {
	int ret = 0;
	while ((size >> ret) > tile_size) {
		ret++;
	}
	return ret;
}

void shrink_pow2(png_byte **dest, int n, int width, int height, png_byte **src) {
    for (int x = 0; x < height; x += n) {
        for (int y = 0; y < width*4; y += n*4) {
            int red = 0;
            int green = 0;
            int blue = 0;
            int alpha = 0;
            for (int i = 0; i < n; i++) {
                for (int j = 0; j < n; j++) {
                    red += src[x+i][y+(j*4)+0];
                    green += src[x+i][y+(j*4)+1];
                    blue += src[x+i][y+(j*4)+2];
                    alpha += src[x+i][y+(j*4)+3];
                }
            }
            dest[x/n][y/n] = red / (n*n);
            dest[x/n][(y/n)+1] = green / (n*n);
            dest[x/n][(y/n)+2] = blue / (n*n);
            dest[x/n][(y/n)+3] = alpha / (n*n);
        }
    }
}