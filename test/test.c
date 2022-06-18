#include "test.h"
#include "lib/library.h"
#include "lib/cformat.h"

CTEST(test_check_dice, even)
{
    
    const int exp = 0;
    const int real = check_dice(dice);
    ASSERT_EQUAL(exp, real);
}

CTEST(test_check_dice, odd)
{
    int dice[SIZE][SIZE]
            = {{1, 2, 3, 4}, {5, 6, 7, 8}, {9, 10, 11, 12}, {13, 15, 14, 0}};
    const int exp = -1;
    const int real = check_dice(dice);
    ASSERT_EQUAL(exp, real);
}