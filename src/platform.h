#ifndef PLATFORM_H
#define PLATFORM_H

#include <stdio.h>

#ifdef _WIN32
#include <direct.h>
#include <fcntl.h>
#include <io.h>
#include <locale.h>
#else
#include <string.h>
#include <sys/stat.h>
#endif

#ifdef DEBUG
#define DEBUG_PRINT(...) print_log(__VA_ARGS__)
#else
#define DEBUG_PRINT(...)
#endif

#ifdef _WIN32
#define CONCAT(x, y) x##y
#define STRING(x)    CONCAT(L, x)
#define FPRINTF(stream, format, ...) \
	fwprintf(stream, STRING(format), __VA_ARGS__)
#define FOPEN(filename, mode) _wfopen(filename, STRING(mode))
#define STRLEN(s)	      (wcslen(s))
#define STRCMP(s1, s2)	      wcscmp(s1, s2)
#else
#define STRING(s)		     s
#define FPRINTF(stream, format, ...) fprintf(stream, format, __VA_ARGS__)
#define FOPEN(filename, mode)	     fopen(filename, mode)
#define STRLEN(s)		     strlen(s)
#define STRCMP(s1, s2)		     strcmp(s1, s2)
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
void print_log(char *format, ...);

#endif /* PLATFORM_H */
