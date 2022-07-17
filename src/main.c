#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <errno.h>
#include <string.h>

#ifdef _WIN32
#define _UNICODE
#include <direct.h>
#include <locale.h>
#include <io.h>
#include <fcntl.h>
#else
#include <sys/stat.h>
#endif

#ifdef _WIN32
#define CONCAT(x, y) x ## y
#define STRING(x) CONCAT(L, x)
#define FPRINTF(stream, format, ...) fwprintf(stream, STRING(format), __VA_ARGS__)
#define FOPEN(filename, mode) _wfopen(filename, STRING(mode))
#else
#define STRING(s) s
#define FPRINTF(stream, format, ...) fprintf(stream, format, __VA_ARGS__)
#define FOPEN(filename, mode) fopen(filename, mode)
#endif

#ifdef _WIN32
int make_directory(const wchar_t *name) {
    return _wmkdir(name);
#else
int make_directory(const char *name) {
    return mkdir(name, 0777); 
#endif
}

#ifdef _WIN32
const wchar_t *getbase(const wchar_t *path) {
    const wchar_t *res = wcsrchr(path, L'\\');
    if (!res)
        res = wcsrchr(path, L'/');
#else
const char *getbase(const char *path) {
    const char *res = strrchr(path, '/');
#endif
    if (!res)
        return path;

    return ++res;
}

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