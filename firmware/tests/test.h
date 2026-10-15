#ifndef TEST_H
#define TEST_H

/* Minimal test helpers: every check prints the failing line, main() returns the failure count. */

#include <stdio.h>
#include <string.h>

static int test_failures;

#define CHECK_INT(expected, actual)                                                                \
    do {                                                                                           \
        const long long e_ = (long long)(expected);                                                \
        const long long a_ = (long long)(actual);                                                  \
        if (e_ != a_) {                                                                            \
            printf("%s:%d: expected %lld, got %lld\n", __FILE__, __LINE__, e_, a_);                \
            test_failures++;                                                                       \
        }                                                                                          \
    } while (0)

#define CHECK_STR(expected, actual)                                                                \
    do {                                                                                           \
        if (strcmp((expected), (actual)) != 0) {                                                   \
            printf("%s:%d: expected \"%s\", got \"%s\"\n", __FILE__, __LINE__, (expected),         \
                   (actual));                                                                      \
            test_failures++;                                                                       \
        }                                                                                          \
    } while (0)

#define TEST_RESULT() (printf("%s: %s\n", __FILE__, test_failures ? "FAILED" : "OK"), test_failures)

#endif /* TEST_H */
