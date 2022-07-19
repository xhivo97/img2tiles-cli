#ifdef _WIN32
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