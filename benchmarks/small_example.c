/*
 * Small example: 2 functions, one leaf + one caller.
 *
 * Intent:
 *   - `abs_value`: leaf function, should be provable with one precondition
 *     on INT_MIN and one postcondition about \result >= 0.
 *   - `clamp_positive`: calls abs_value, then clamps to an upper bound.
 *     Should be provable using abs_value's published postcondition plus a
 *     bound precondition.
 *
 * This exercises:
 *   - call-site precondition VO (clamp_positive → abs_value)
 *   - RTE VO (INT_MIN negation in abs_value)
 *   - transitive dirty / GLOBAL_OK propagation
 */

#include <limits.h>

int abs_value(int x)
{
    if (x < 0) {
        return -x;
    }
    return x;
}

int clamp_positive(int x, int upper)
{
    int a = abs_value(x);
    if (a > upper) {
        return upper;
    }
    return a;
}
