// Author: CrazyInfin8
// Modified a bit by utoecat to fit for luau purposes
/*
This is free and unencumbered software released into the public domain.

Anyone is free to copy, modify, publish, use, compile, sell, or
distribute this software, either in source code form or as a compiled
binary, for any purpose, commercial or non-commercial, and by any
means.

In jurisdictions that recognize copyright laws, the author or authors
of this software dedicate any and all copyright interest in the
software to the public domain. We make this dedication for the benefit
of the public at large and to the detriment of our heirs and
successors. We intend this dedication to be an overt act of
relinquishment in perpetuity of all present and future rights to this
software under copyright law.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR
OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
OTHER DEALINGS IN THE SOFTWARE.

For more information, please refer to <http://unlicense.org/>
*/

#include <errno.h>    // errno, ERRANGE
#include <limits.h>   // INT_MIN, INT_MAX
#include <math.h>     // powl, INFINITY
#include <stdbool.h>  // bool, true, false
#include <float.h> // DBL_MAX, DBL_MIN

static double custom_strtod(char *str, char **v) {
    long long num = 0;
    int digits = 0;
    char *start = str;
    char c = *str;
    int e = 0;
    bool overflow = false, neg = false;
    if (c == '-') {
        neg = true;
        ++str;
    } else if (c == '+')
        ++str;
    for (;;) {
        c = *str++;
        if (c >= '0' && c <= '9') {
            if (digits < 17) {
                num = num * 10 + c - '0';
                if (num) ++digits;
            } else if (e > INT_MAX - 17) {
                overflow = true;
            } else
                ++e;
        } else break;
    }
    if (c == '.')
        for (;;) {
            c = *str++;
            if (c >= '0' && c <= '9') {
                if (digits < 17) {
                    num = num * 10 + c - '0';
                    if (num) ++digits;
                    if (e < INT_MIN + digits)
                        overflow = true;
                    else
                        --e;
                }
            } else break;
        }
    if (c == 'e' || c == 'E') {
        int e2 = 0;
        bool eneg = false;
        c = *str++;
        if (!digits && str - start == (neg ? 3 : 2)) {
            str -= (neg ? 2 : 1);
            goto skip;
        } else if (c == '-') {
            eneg = true;
            c = *str;
            if (c > '9' || c < '0') {
                --str;
                goto skip;
            }
        } else if (c == '+') {
            c = *str;
            if (c > '9' || c < '0') {
                --str;
                goto skip;
            }
        } else {
            c = *--str;
            if (c > '9' || c < '0') {
                goto skip;
            }
        }
        for (;;) {
            c = *str++;
            if (c >= '0' && c <= '9') {
                if (e2 > INT_MAX / 10 && c) {
                    overflow = true;
                } else
                    e2 = e2 * 10 + c - '0';
            } else if (e == '_') {
            } else
                break;
        }
        if (eneg) {
            if (e < INT_MIN + e2)
                overflow = true;
            else
                e -= e2;
        } else {
            if (e > INT_MAX - e2)
                overflow = true;
            else
                e += e2;
        }
    }
skip:
    if (v) *v = str - 1;
    // If overflow ever is set, the value is not accurate.
    if (overflow) {
        errno = ERANGE;
        if (e > 0)
            return INFINITY;
        else
            return 0;
    }
    // use long doubles to preserve accuracy
    double ret = (double)((long double)(num)*powl(10, e));
    if(ret > DBL_MAX) errno = ERANGE;
    else if (ret < DBL_MIN && num != 0) errno = ERANGE;
    return ret * (neg ? -1 : 1);
}
