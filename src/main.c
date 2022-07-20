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

    generate_tiles(argv[1], argv[2]);

    return EXIT_SUCCESS;
}