#include <stdio.h>
#include <stdlib.h>
#include <png.h>
#include "image.h"

#ifdef _WIN32
static const wchar_t *valid_extensions[] = {L".png", L".PNG", L".jpg", L".JPG", L".jpeg", L".JPEG"};
#else
static const char *valid_extensions[] = {".png", ".PNG", ".jpg", ".JPG", ".jpeg", ".JPEG"};
#endif

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

    if (check_extension(in_path) == EXIT_FAILURE) {
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

int check_extension(const char *in_path) {
    if (STRLEN(in_path) < 5)
        return EXIT_FAILURE;

    const char *ext = in_path + STRLEN(in_path)-5;
    int size = sizeof(valid_extensions) / sizeof(valid_extensions[0]);

    if (ext[0] == '.') {
        for (int i = 0; i < size; i++) {
            if (strcmp(ext, valid_extensions[i]) == 0)
                return EXIT_SUCCESS;
        }
    } else {
        ext++;
        for (int i = 0; i < size; i++) {
            if (strcmp(ext, valid_extensions[i]) == 0)
                return EXIT_SUCCESS;
        }
    }
    
    return EXIT_FAILURE;
}