/*
    Char Utility Macros
    Author: Brian Khuu (2025)

    # MIT License

    Copyright (c) 2025 Brian Khuu

    Permission is hereby granted, free of charge, to any person obtaining a copy
    of this software and associated documentation files (the "Software"), to deal
    in the Software without restriction, including without limitation the rights
    to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
    copies of the Software, and to permit persons to whom the Software is
    furnished to do so, subject to the following conditions:

    The above copyright notice and this permission notice shall be included in all
    copies or substantial portions of the Software.

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
    AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
    OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
    SOFTWARE.

 */

/* Design Notes:
 * The below macros are focused on being compact even if optimisation is
 * disabled by relying only on arithmic operation and comparison, rather
 * than the more computationally expensive branching instructions.
 *
 * In addition there are some macros with a grokkable version included
 * with the same behavior but with clearer syntax. They will still
 * compile to the same size if optimisation is enabled. In practice for
 * many projects, especially in embedded compilers, optimisation may be
 * unused due to being locked behind a paywall or being hard to use
 * without hitting compiler optimisation bugs...
 *
 * */

#include <stdbool.h>

/* ==========================
 * Set Sizes
 * ========================== */

int CHARUTIL_BINARY_COUNT(void) {
    return 2;
}

int CHARUTIL_BINARY_COUNT_GROKKABLE(void) {
    return (int)(sizeof("01") - 1);
}

int CHARUTIL_OCTAL_COUNT(void) {
    return 8;
}

int CHARUTIL_OCTAL_COUNT_GROKKABLE(void) {
    return (int)(sizeof("01234567") - 1);
}

int CHARUTIL_DIGIT_COUNT(void) {
    return 10;
}

int CHARUTIL_DIGIT_COUNT_GROKKABLE(void) {
    return (int)(sizeof("0123456789") - 1);
}

int CHARUTIL_HEX_COUNT(void) {
    return 16;
}

int CHARUTIL_HEX_COUNT_GROKKABLE(void) {
    return (int)(sizeof("0123456789ABCDEF") - 1);
}

int CHARUTIL_ALPHABET_COUNT(void) {
    return 26;
}

int CHARUTIL_ALPHABET_COUNT_GROKKABLE(void) {
    return (int)(sizeof("abcdefghijklmnopqrstuvwxyz") - 1);
}

int CHARUTIL_ALPHANUMERIC_COUNT(void) {
    return 36;
}

int CHARUTIL_ALPHANUMERIC_COUNT_GROKKABLE(void) {
    return (int)(sizeof("0123456789abcdefghijklmnopqrstuvwxyz") - 1);
}

/* ==========================
 * Character Type Checks
 * ========================== */

bool IS_BINARY(char ch) {
    return (unsigned)(ch - '0') < CHARUTIL_BINARY_COUNT();
}

bool IS_OCTAL(char ch) {
    return (unsigned)(ch - '0') < CHARUTIL_OCTAL_COUNT();
}

bool IS_DIGIT(char ch) {
    return (unsigned)(ch - '0') < CHARUTIL_DIGIT_COUNT();
}

bool IS_LOWER(char ch) {
    return (unsigned)(ch - 'a') < CHARUTIL_ALPHABET_COUNT();
}

bool IS_UPPER(char ch) {
    return (unsigned)(ch - 'A') < CHARUTIL_ALPHABET_COUNT();
}

bool IS_ALPHA(char ch) {
    return IS_LOWER(ch) || IS_UPPER(ch);
}

bool IS_ALNUM(char ch) {
    return IS_DIGIT(ch) || IS_ALPHA(ch);
}

bool IS_HEX_DIGIT(char ch) {
    return IS_DIGIT(ch) || (unsigned)(ch - 'A') < 6 || (unsigned)(ch - 'a') < 6;
}

bool IS_PRINTABLE(char ch) {
    return (unsigned)(ch - 32) < 95;
}

bool IS_ASCII(char ch) {
    return (unsigned) ch < 128;
}

bool IS_EXTENDED_ASCII(char ch) {
    return (unsigned) ch < 256;
}

bool IS_SPACE(char ch) {
    return ch == ' ' || ch == '\t' || ch == '\n' || ch == '\r' || ch == '\f' || ch == '\v';
}

bool IS_PUNCT(char ch) {
    return IS_PRINTABLE(ch) && !(IS_ALNUM(ch) || IS_SPACE(ch));
}

bool IS_BRACKET(char ch) {
    return ch == '(' || ch == ')' || ch == '{' || ch == '}' || ch == '[' || ch == ']';
}

bool IS_SYMBOL(char ch) {
    return IS_PUNCT(ch) && !IS_BRACKET(ch);
}

bool IS_BINARY_GROKKABLE(char ch) {
    return ch == '0' || ch == '1';
}

bool IS_OCTAL_GROKKABLE(char ch) {
    return ch >= '0' && ch <= '7';
}

bool IS_DIGIT_GROKKABLE(char ch) {
    return '0' <= ch && ch <= '9';
}

bool IS_LOWER_GROKKABLE(char ch) {
    return 'a' <= ch && ch <= 'z';
}

bool IS_UPPER_GROKKABLE(char ch) {
    return 'A' <= ch && ch <= 'Z';
}

bool IS_ALPHA_GROKKABLE(char ch) {
    return IS_LOWER_GROKKABLE(ch) || IS_UPPER_GROKKABLE(ch);
}

bool IS_ALNUM_GROKKABLE(char ch) {
    return IS_DIGIT_GROKKABLE(ch) || IS_LOWER_GROKKABLE(ch) || IS_UPPER_GROKKABLE(ch);
}

bool IS_HEX_DIGIT_GROKKABLE(char ch) {
    return ('0' <= ch && ch <= '9') || ('a' <= ch && ch <= 'f') || ('A' <= ch && ch <= 'F');
}

bool IS_PRINTABLE_GROKKABLE(char ch) {
    return ' ' <= ch && ch <= '~';
}

/* ==========================
 * Digit & Hex Conversions
 * ========================== */

int ASCII_TO_BINARY(char ch, int DEFAULT) {
    return (ch == '0' || ch == '1') ? (ch - '0') : DEFAULT;
}

char BINARY_TO_ASCII(int num, char DEFAULT) {
    return (unsigned) num < 2 ? (char)(num + '0') : DEFAULT;
}

int FAST_ASCII_TO_BINARY(char ch) {
    return ch - '0';
}

char FAST_BINARY_TO_ASCII(int num) {
    return (char)(num + '0');
}

int ASCII_TO_OCTAL(char ch, int DEFAULT) {
    return ('0' <= ch && ch <= '7') ? (ch - '0') : DEFAULT;
}

char OCTAL_TO_ASCII(int num, char DEFAULT) {
    return (unsigned) num < 8 ? (char)(num + '0') : DEFAULT;
}

int FAST_ASCII_TO_OCTAL(char ch) {
    return ch - '0';
}

char FAST_OCTAL_TO_ASCII(int num) {
    return (char)(num + '0');
}

int ASCII_TO_DIGIT(char ch) {
    return ('0' <= ch && ch <= '9') ? (ch - '0') : -1;
}

char DIGIT_TO_ASCII(int num, char DEFAULT) {
    return (unsigned) num < 10 ? (char)(num + '0') : DEFAULT;
}

char FAST_DIGIT_TO_ASCII(int num) {
    return (char)(num + '0');
}

int FAST_ASCII_TO_DIGIT(char ch) {
    return ch - '0';
}

int HEX_TO_INT(char ch, int DEFAULT) {
    if ('0' <= ch && ch <= '9') return ch - '0';
    if ('A' <= ch && ch <= 'F') return ch - 'A' + 10;
    if ('a' <= ch && ch <= 'f') return ch - 'a' + 10;
    return DEFAULT;
}

int FAST_UPPERCASE_HEX_TO_INT(char ch) {
    return ('0' <= ch && ch <= '9') ? (ch - '0') : (ch - 'A' + 10);
}

int FAST_LOWERCASE_HEX_TO_INT(char ch) {
    return ('0' <= ch && ch <= '9') ? (ch - '0') : (ch - 'a' + 10);
}

char NIBBLE_TO_UPPERCASE_HEX(int n, char DEFAULT) {
    return (unsigned) n < 16 ? (char)(n < 10 ? (n + '0') : (n - 10 + 'A')) : DEFAULT;
}

char NIBBLE_TO_LOWERCASE_HEX(int n, char DEFAULT) {
    return (unsigned) n < 16 ? (char)(n < 10 ? (n + '0') : (n - 10 + 'a')) : DEFAULT;
}

char FAST_NIBBLE_TO_UPPERCASE_HEX(int n) {
    return (char)(n < 10 ? (n + '0') : (n - 10 + 'A'));
}

char FAST_NIBBLE_TO_LOWERCASE_HEX(int n) {
    return (char)(n < 10 ? (n + '0') : (n - 10 + 'a'));
}