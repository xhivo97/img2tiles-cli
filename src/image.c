#include <stdio.h>
#include <stdlib.h>
#include <png.h>
#include "image.h"

enum extension { IMAGE_NOT_SUPPORTED, IMAGE_PNG, IMAGE_JPEG };

struct image {
    enum extension ext;
};

#ifdef _WIN32
static const wchar_t *valid_extensions[] = {L".png", L".PNG", L".jpg", L".JPG", L".jpeg", L".JPEG"};
#else
static const char *valid_extensions[] = {".png", ".PNG", ".jpg", ".JPG", ".jpeg", ".JPEG"};
#endif

#ifdef _WIN32
int generate_tiles(const wchar_t *in_path, const wchar_t *out_dir) {
#else
int generate_tiles(const char *in_path, const char *out_dir) {
    struct image img;
#endif
    FILE *fp = FOPEN(in_path, "rb");
    if (!fp) {
        print_error("could not open: %s\n", getbase(in_path));
        return EXIT_FAILURE;
    }

    img.ext = check_extension(in_path);
    if (img.ext == IMAGE_NOT_SUPPORTED) {
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