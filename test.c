/**
 * Testing c8_strtod
 */
#include <stdio.h>   // printf
#include <stdlib.h>  // strtod

#include "strtod.c"  // c8_strtod
#define testCount 32
char *values[testCount] = {
    /* 0*/
    "17976931348623158000000000000000000000000000000000000000000000000000000000"
    "00000000000000000000000000000000000000000000000000000000000000000000000000"
    "00000000000000000000000000000000000000000000000000000000000000000000000000"
    "00000000000000000000000000000000000000000000000000000000000000000000000000"
    "0000000000000junk",
    /* 1*/
    "17976931348623158000000000000000000000000000000000000000000000000000000000"
    "00000000000000000000000000000000000000000000000000000000000000000000000000"
    "00000000000000000000000000000000000000000000000000000000000000000000000000"
    "00000000000000000000000000000000000000000000000000000000000000000000000000"
    "00000000000000",
    /* 2*/
    "-1797693134862315800000000000000000000000000000000000000000000000000000000"
    "00000000000000000000000000000000000000000000000000000000000000000000000000"
    "00000000000000000000000000000000000000000000000000000000000000000000000000"
    "00000000000000000000000000000000000000000000000000000000000000000000000000"
    "000000000000000",
    /* 3*/
    "0."
    "00000000000000000000000000000000000000000000000000000000000000000000000000"
    "00000000000000000000000000000000000000000000000000000000000000000000000000"
    "00000000000000000000000000000000000000000000000000000000000000000000000000"
    "00000000000000000000000000000000000000000000000000000000000000000000000000"
    "0000000000022250738585072014",
    /* 4*/
    "0."
    "00000000000000000000000000000000000000000000000000000000000000000000000000"
    "00000000000000000000000000000000000000000000000000000000000000000000000000"
    "00000000000000000000000000000000000000000000000000000000000000000000000000"
    "00000000000000000000000000000000000000000000000000000000000000000000000000"
    "00000000000022250738585072014",
    /* 5*/
    "0."
    "00000000000000000000000000000000000000000000000000000000000000000000000000"
    "00000000000000000000000000000000000000000000000000000000000000000000000000"
    "00000000000000000000000000000000000000000000000000000000000000000000000000"
    "00000000000000000000000000000000000000000000000000000000000000000000000000"
    "0000000000000000000000000022250738585072014",
    /* 6*/
    "0."
    "00000000000000000000000000000000000000000000000000000000000000000000000000"
    "00000000000000000000000000000000000000000000000000000000000000000000000000"
    "00000000000000000000000000000000000000000000000000000000000000000000000000"
    "00000000000000000000000000000000000000000000000000000000000000000000000000"
    "00000000000000000000000000022250738585072014",
    /* 7*/ "2.55e2",
    /* 8*/ "1.7976931348623158e+308",
    /* 9*/ "2.2250738585072014e-308",
    /*10*/ "2.5e-323",
    /*11*/ "25500e-2",
    /*12*/ "3.14159265358979323846264338327950626",
    /*13*/ "314159265358979323846264338327950626e-35",
    /*14*/ "7e+",
    /*15*/ "7e-",
    /*16*/ "7e",
    /*17*/ "e55",
    /*18*/ "888.888",
    /*19*/ "888.888e-2",
    /*20*/ "888.888e2",
    /*21*/ "0.000000000000000000000000000000000000783475",
    /*22*/ "0e21",
    /*23*/ "-e42",
    /*24*/ "-e+",
    /*25*/ "-123.456e78",
    /*26*/ "-0e78",
    /*27*/ "-0e78",
    /*28*/ "-e",
    /*29*/ "-27e+",
    /*30*/ "1_000_000",
    /*31*/ "1.000_000_1",
};

#include <string.h>  // strcmp
void consistencyCheck() {
    bool success = true;
    for (int i = 0; i < testCount; ++i) {
        char *str1, *str2;
        int err1, err2;
        errno = 0;
        double d1 = strtod(values[i], &str1);
        err1 = errno;
        errno = 0;
        double d2 = c8_strToD(values[i], &str2);
        err2 = errno;
        if (d1 != d2 || strcmp(str1, str2) || err1 != err2) {
            printf(
                "%2d: %.20e (remaining: \"%s\", errcode: %d)\t%.20e (remaining: \"%s\", errcode: %d)\n",
                i, d1, str1, err1, d2, str2, err2);
            success = false;
        }
    }
    if (success) {
        printf("All tests match!\n");
    } else {
        printf("values printed above don't match\n");
    }
}

/* Implement get_current_time() depending on the OS. */
#if defined(WIN32) || defined(__WIN32) || defined(_WIN32) || \
    defined(__WIN32__) || defined(_WIN32_)

#include <windows.h> // QueryPerformanceCounter, QueryPerformanceFrequency

double get_current_time() {
    LARGE_INTEGER t, f;
    QueryPerformanceCounter(&t);
    QueryPerformanceFrequency(&f);
    return (double)t.QuadPart / (double)f.QuadPart;
}

#else

#include <sys/resource.h> // timeval
#include <sys/time.h> // gettimeofday

double get_current_time() {
    struct timeval t;
    gettimeofday(&t, 0);
    return t.tv_sec + t.tv_usec * 1e-6;
}

#endif

int getTime() {
    static double start, end;
    double delta;
    end = get_current_time();
    delta = end - start;
    start = end;
    return delta * 1000000000;
}

void benchmark() {
    double returns1[testCount] = {0};
    double returns2[testCount] = {0};
    getTime();
    for (int i = 0; i < testCount; ++i) {
        returns1[i] = strtod(values[i], NULL);
    }
    int ogStrToD = getTime();

    getTime();
    for (int i = 0; i < testCount; ++i) {
        returns2[i] = c8_strToD(values[i], NULL);
    }
    int newStrToD = getTime();
    printf(
        "Benchmark results: (lower is better)\n"
        "original strtod: %d\n"
        "new strtod:      %d\n"
        "Printing results of functions:\n",
        ogStrToD, newStrToD);
    for (int i = 0; i < 22; ++i) {
        printf("%2d: %.20e\t%.20e\n", i, returns1[i], returns2[i]);
    }
}

int main(int argc, char const *argv[]) {
    consistencyCheck();
    benchmark();
    return 0;
}
