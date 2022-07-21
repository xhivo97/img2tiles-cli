#include<stdio.h> 
#include<stdarg.h> 

#ifdef _WIN32
#include<stdlib.h>
#include <direct.h>
#include <locale.h>
#include <io.h>
#include <fcntl.h>
#else
#include <sys/stat.h>
#include <string.h>
#endif

#include "platform.h"

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

void print_error(char *fmt, ...) {
    va_list arg;
    va_start(arg, fmt);

#ifdef _WIN32
    wchar_t *w_fmt;
    int size = snwprintf(NULL, 0, L"%s", fmt);
    w_fmt = malloc(sizeof(wchar_t)*size+1);
    snwprintf(w_fmt, size, L"%s", fmt);

    fmt_s_to_S(w_fmt);
    vfwprintf(stderr, w_fmt, arg);
#else
    vfprintf(stderr, fmt, arg);
#endif

    va_end(arg);
}

void print_log(char *fmt, ...) {
    va_list arg;
    va_start(arg, fmt);

#ifdef _WIN32
    wchar_t *w_fmt;
    int size = snwprintf(NULL, 0, L"%s", fmt);
    w_fmt = malloc(sizeof(wchar_t)*size+1);
    snwprintf(w_fmt, size, L"%s", fmt);

    fmt_s_to_S(w_fmt);
    vfwprintf(stdout, w_fmt, arg);
#else
    vfprintf(stdout, fmt, arg);
#endif

    va_end(arg);
}

#ifdef _WIN32
void fmt_s_to_S(wchar_t *fmt) {
    for (size_t i = 0; i < wcslen(fmt); i++) {
        if(fmt[i] == L'%' && fmt[i+1] == L's') {
            fmt[i+1] = L'S';
        }
    }
}
#endif
