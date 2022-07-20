#ifndef PLATFORM_H
#define PLATFORM_H

#include <stdio.h>

#ifdef _WIN32
#include <direct.h>
#include <locale.h>
#include <io.h>
#include <fcntl.h>
#else
#include <sys/stat.h>
#include <string.h>
#endif

#ifdef _WIN32
#define CONCAT(x, y) x ## y
#define STRING(x) CONCAT(L, x)
#define FPRINTF(stream, format, ...) fwprintf(stream, STRING(format), __VA_ARGS__)
#define FOPEN(filename, mode) _wfopen(filename, STRING(mode))
#define STRLEN(s) (wcslen(s)+1)
#else
#define STRING(s) s
#define FPRINTF(stream, format, ...) fprintf(stream, format, __VA_ARGS__)
#define FOPEN(filename, mode) fopen(filename, mode)
#define STRLEN(s) strlen(s)
#endif

#ifdef _WIN32
void fmt_s_to_S(wchar_t *fmt);
int make_directory(const wchar_t *name);
const wchar_t *getbase(const wchar_t *path);
#else
int make_directory(const char *name);
const char *getbase(const char *path);
#endif

void print_error(char *format, ...);

#endif /* PLATFORM_H */