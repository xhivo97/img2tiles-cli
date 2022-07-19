#include <stdio.h>
#include <stdlib.h>
#include <png.h>
#include "image.h"

#ifdef _WIN32
int generate_tiles(const wchar_t *in_path, const wchar_t *out_dir) {
#else
int generate_tiles(const char *in_path, const char *out_dir) {
#endif
    FILE *fp = FOPEN(in_path, "rb");
    if (!fp) {
        FPRINTF(stderr, "could not open: %s\n", in_path);
        return EXIT_FAILURE;
    }

    if (make_directory(out_dir) != 0) {
        FPRINTF(stderr, "could not create directory: %s\n", out_dir);
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}