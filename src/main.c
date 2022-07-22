#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

#include "image.h"
#include "platform.h"

#define MAX_FILEPATH 4096

#ifdef _WIN32
int wmain(int argc, wchar_t **argv) {
    setlocale(LC_ALL, "");

    _setmode(_fileno(stdout), _O_U16TEXT);
    _setmode(_fileno(stderr), _O_U16TEXT);
#else
int main(int argc, char **argv) {
#endif

    if (argc != 3) {
        print_error("usage: %s <input_file> <output_folder>\n", getbase(argv[0]));
        return EXIT_FAILURE;
    }

    struct image img;
    if (image_struct_init(&img, argv[1], argv[2]) == EXIT_FAILURE)
        return EXIT_FAILURE;

    if (image_init_read(&img) == EXIT_FAILURE) {
        print_error("could not initialize image reader");
        return EXIT_FAILURE;
    }

    if (image_read_rows(256, &img)) {
        print_error("could not read rows");
        return EXIT_FAILURE;
    }
    image_struct_destroy(&img);
    return EXIT_SUCCESS;
}