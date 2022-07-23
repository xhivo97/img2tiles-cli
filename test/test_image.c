#include <assert.h>
#include "../src/image.h"
#include "../src/platform.h"


void test_check_extension() {
    print_log("running test_check_extensions\n");
    // File too short
    assert(check_extension(STRING("a.pn")) == IMAGE_NOT_SUPPORTED);

    // Short filenames valid png
    assert(check_extension(STRING("a.png")) == IMAGE_PNG);
    assert(check_extension(STRING("a.PNG")) == IMAGE_PNG);

    // Long filenames valid png
    assert(check_extension(STRING("/this/is/a/valid/image.png")) == IMAGE_PNG);
    assert(check_extension(STRING("/this/is/a/valid/image.PNG")) == IMAGE_PNG);

    // Short filenames valid jpeg
    assert(check_extension(STRING("a.jpg")) == IMAGE_JPEG);
    assert(check_extension(STRING("a.jpeg")) == IMAGE_JPEG);
    assert(check_extension(STRING("a.jpg")) == IMAGE_JPEG);
    assert(check_extension(STRING("a.JPG")) == IMAGE_JPEG);

    // Long filenames valid jpeg
    assert(check_extension(STRING("/this/is/a/valid/image.jpg")) == IMAGE_JPEG);
    assert(check_extension(STRING("/this/is/a/valid/image.jpeg")) == IMAGE_JPEG);
    assert(check_extension(STRING("/this/is/a/valid/image.JPG")) == IMAGE_JPEG);
    assert(check_extension(STRING("/this/is/a/valid/image.JPEG")) == IMAGE_JPEG);

    // Invalid extensions
    assert(check_extension(STRING("/not/valid.inv")) == IMAGE_NOT_SUPPORTED);

    // Invalid no extension
    assert(check_extension(STRING("/not/png")) == IMAGE_NOT_SUPPORTED);
    assert(check_extension(STRING("/not/jpg")) == IMAGE_NOT_SUPPORTED);

    print_log("check_extension tests passed\n");
}

#ifdef _WIN32
int wmain() {
#else
int main() {
#endif
    test_check_extension();
    print_log("all tests passed\n");
    return 0;
}