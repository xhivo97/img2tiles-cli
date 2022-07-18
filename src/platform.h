#ifndef PLATFORM_H
#define PLATFORM_H

#ifdef _WIN32
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
#define STRSIZE(s) (wcslen(s)+1)*sizeof(wchar_t)
#else
#define STRING(s) s
#define FPRINTF(stream, format, ...) fprintf(stream, format, __VA_ARGS__)
#define FOPEN(filename, mode) fopen(filename, mode)
#define STRSIZE(s) strlen(s)*sizeof(char)
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

#endif /* PLATFORM_H */