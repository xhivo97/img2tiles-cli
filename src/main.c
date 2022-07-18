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

    const wchar_t *input = NULL;
    const wchar_t *output = NULL;
#else
int main(int argc, char **argv) {
    const char *input = NULL;
    const char *output = NULL;
#endif

    if (argc != 3) {
        FPRINTF(stderr, "usage: %s <input_file> <output_folder>\n", getbase(argv[0]));
        return EXIT_FAILURE;
    }

    input = argv[1];
    output = argv[2];

    generate_tiles(input, output);

    FPRINTF(stdout, "Size of first argument is: %zu\n", STRSIZE(input));
    FILE *fp = FOPEN(input, "rb");
    if (!fp) {
        FPRINTF(stderr, "could not open: %s\n", input);
        return EXIT_FAILURE;
    }

    if (make_directory(output) != 0) {
        FPRINTF(stderr, "could not create directory: %s\n", output);
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}