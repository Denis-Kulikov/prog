#define CTEST_MAIN
#define CTEST_SEGFAULT
#define CTEST_COLOR_OK

#include "../thirdparty/ctest.h"

int main(int argc, const char** argv)
{
    return ctest_main(argc, argv);
}