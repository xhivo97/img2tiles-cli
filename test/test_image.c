#include <assert.h>
#include "../src/image.h"

void test_check_extension() {
    printf("running test_check_extensions\n");
    // File too short
    assert(check_extension("a.pn") == IMAGE_NOT_SUPPORTED);

    // Short filenames valid png
    assert(check_extension("a.png") == IMAGE_PNG);
    assert(check_extension("a.PNG") == IMAGE_PNG);

    // Long filenames valid png
    assert(check_extension("/this/is/a/valid/image.png") == IMAGE_PNG);
    assert(check_extension("/this/is/a/valid/image.PNG") == IMAGE_PNG);

    // Short filenames valid jpeg
    assert(check_extension("a.jpg") == IMAGE_JPEG);
    assert(check_extension("a.jpeg") == IMAGE_JPEG);
    assert(check_extension("a.jpg") == IMAGE_JPEG);
    assert(check_extension("a.JPG") == IMAGE_JPEG);

    // Long filenames valid jpeg
    assert(check_extension("/this/is/a/valid/image.jpg") == IMAGE_JPEG);
    assert(check_extension("/this/is/a/valid/image.jpeg") == IMAGE_JPEG);
    assert(check_extension("/this/is/a/valid/image.JPG") == IMAGE_JPEG);
    assert(check_extension("/this/is/a/valid/image.JPEG") == IMAGE_JPEG);

    // Invalid extensions
    assert(check_extension("/not/valid.inv") == IMAGE_NOT_SUPPORTED);

    // Invalid no extension
    assert(check_extension("/not/png") == IMAGE_NOT_SUPPORTED);
    assert(check_extension("/not/jpg") == IMAGE_NOT_SUPPORTED);

    printf("check_extension tests passed\n");
}

int main() {
    test_check_extension();
    printf("all tests passed\n");
    return 0;
}