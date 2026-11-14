// stb_divide.h - v0.94 - public domain - Sean Barrett, Feb 2010
// Three kinds of divide/modulus of signed integers.
//
// HISTORY
//
//   v0.94              Fix integer overflow issues
//   v0.93  2020-02-02  Write useful exit() value from main()
//   v0.92  2019-02-25  Fix warning
//   v0.91  2010-02-27  Fix euclidean division by INT_MIN for non-truncating C
//                      Check result with 64-bit math to catch such cases
//   v0.90  2010-02-24  First public release
//
// USAGE
//
// In *ONE* source file, put:
//
//    #define STB_DIVIDE_IMPLEMENTATION
//    // #define C_INTEGER_DIVISION_TRUNCATES  // see Note 1
//    // #define C_INTEGER_DIVISION_FLOORS     // see Note 2
//    #include "stb_divide.h"
//
// Other source files should just include stb_divide.h
//
// Note 1: On platforms/compilers that you know signed C division
// truncates, you can #define C_INTEGER_DIVISION_TRUNCATES.
//
// Note 2: On platforms/compilers that you know signed C division
// floors (rounds to negative infinity), you can #define
// C_INTEGER_DIVISION_FLOORS.
//
// You can #define STB_DIVIDE_TEST in which case the implementation
// will generate a main() and compiling the result will create a
// program that tests the implementation. Run it with no arguments
// and any output indicates an error; run it with any argument and
// it will also print the test results. Define STB_DIVIDE_TEST_64
// to a 64-bit integer type to avoid overflows in the result-checking
// which give false negatives.
//
// ABOUT
//
// This file provides three different consistent divide/mod pairs
// implemented on top of arbitrary C/C++ division, including correct
// handling of overflow of intermediate calculations:
//
//     trunc:   a/b truncates to 0,           a%b has same sign as a
//     floor:   a/b truncates to -inf,        a%b has same sign as b
//     eucl:    a/b truncates to sign(b)*inf, a%b is non-negative
//
// Not necessarily optimal; I tried to keep it generally efficient,
// but there may be better ways.
//
// Briefly, for those who are not familiar with the problem, we note
// the reason these divides exist and are interesting:
//
//     'trunc' is easy to implement in hardware (strip the signs,
//          compute, reapply the signs), thus is commonly defined
//          by many languages (including C99)
//
//     'floor' is simple to define and better behaved than trunc;
//          for example it divides integers into fixed-size buckets
//          without an extra-wide bucket at 0, and for a fixed
//          divisor N there are only |N| possible moduli.
//
//     'eucl' guarantees fixed-sized buckets *and* a non-negative
//          modulus and defines division to be whatever is needed
//          to achieve that result.
//
// See "The Euclidean definition of the functions div and mod"
// by Raymond Boute (1992), or "Division and Modulus for Computer
// Scientists" by Daan Leijen (2001)
//
// We assume of the built-in C division:
//     (a) modulus is the remainder for the corresponding division
//     (b) a/b truncates if a and b are the same sign
//
// Property (a) requires (a/b)*b + (a%b)==a, and is required by C.
// Property (b) seems to be true of all hardware but is *not* satisfied
// by the euclidean division operator we define, so it's possibly not
// always true. If any such platform turns up, we can add more cases.
// (Possibly only stb_div_trunc currently relies on property (b).)
//
// LICENSE
//
//   See end of file for license information.


#define INT_MIN -2147483648



int stb__div(int v1, int v2)
{
    int q = v1/v2, r = v1%v2;
    if ((r > 0 && v2 < 0) || (r < 0 && v2 > 0))
        return q-1;
    else
        return q;
}



int stb__mod(int v1, int v2)
{
    int r = v1%v2;
    if ((r > 0 && v2 < 0) || (r < 0 && v2 > 0))
        return r+v2;
    else
        return r;
}



int stb_div_trunc(int v1, int v2)
{
    if (v1 >= 0 && v2 <= 0)
        return -stb__div(-v1,v2);  // both negative to avoid overflow
    if (v1 <= 0 && v2 >= 0)
        if (v1 != INT_MIN)
            return -stb__div(v1,-v2);    // both negative to avoid overflow
        else
            return -stb__div(v1+v2,-v2)-1; // push v1 away from wrap point
    else
        return v1/v2;            // same sign, so expect truncation
}



int stb_div_eucl(int v1, int v2)
{
    int q,r;
    // handle every quadrant separately, since we can't rely on q and r flor
    if (v1 >= 0)
        if (v2 >= 0)
            return stb__div(v1,v2);
        else if (v2 != INT_MIN)
            q = -stb__div(v1,-v2), r = stb__mod(v1,-v2);
        else
            q = 0, r = v1;
    else if (v1 != INT_MIN)
        if (v2 >= 0)
            q = -stb__div(-v1,v2), r = -stb__mod(-v1,v2);
        else if (v2 != INT_MIN)
            q = stb__div(-v1,-v2), r = -stb__mod(-v1,-v2);
        else // if v2 is INT_MIN, then we can't use -v2, but we can't divide by v2
            q = 1, r = v1-q*v2;
    else // if v1 is INT_MIN, we have to move away from overflow place
        if (v2 >= 0)
            q = -stb__div(-(v1+v2),v2)-1, r = -stb__mod(-(v1+v2),v2);
        else if (v2 != INT_MIN)
            q = stb__div(-(v1-v2),-v2)+1, r = -stb__mod(-(v1-v2),-v2);
        else // for INT_MIN / INT_MIN, we need to be extra-careful to avoid overflow
            q = 1, r = 0;
    if (r >= 0)
        return q;
    else
        return q + (v2 > 0 ? -1 : 1);
}



int stb_mod_trunc(int v1, int v2)
{
    if (v1 >= 0) { // modulus result should always be positive
        int r = stb__mod(v1,v2);
        if (r >= 0)
            return r;
        else
            return r - (v2 < 0 ? v2 : -v2);
    } else {    // modulus result should always be negative
        int r = stb__mod(v1,v2);
        if (r <= 0)
            return r;
        else
            return r + (v2 < 0 ? v2 : -v2);
    }
}



int stb_mod_floor(int v1, int v2)
{
    if (v2 >= 0) { // result should always be positive
        int r = stb__mod(v1,v2);
        if (r >= 0)
            return r;
        else
            return r + v2;
    }
    else { // result should always be negative
        int r = stb__mod(v1,v2);
        if (r <= 0)
            return r;
        else
            return r + v2;
    }
}



int stb_mod_eucl(int v1, int v2)
{
   int r = stb__mod(v1,v2);

   if (r >= 0)
      return r;
   else
      return r - (v2 < 0 ? v2 : -v2); // negative abs() [to avoid overflow]
}