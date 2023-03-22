#ifndef UNIT_TEST_H
#define UNIT_TEST_H

#include <stdio.h>

#define KNRM "\x1b[0m"
#define KRED  "\x1b[31m"
#define KGRN  "\x1b[32m"
#define KBLD  "\x1b[1m"

#define PASS KGRN "PASS" KNRM
#define FAIL KRED "FAIL" KNRM

#define UNIT_TEST(x)                            \
    static void utest_ ## x(void)

#define RUN_TEST(x)                                    \
    do {                                                  \
        fprintf(stdout, KBLD "Running %s..." KNRM "\n", #x);             \
        utest_ ## x();                                          \
    } while(0)

#define u_assert(x)                                                     \
    ((x)                                                                  \
   ? (void)fprintf(stdout, "%s:%d \'%s\' " PASS "\n",          \
                   __FILE__, __LINE__, #x)                              \
   : (void)fprintf(stdout, "%s:%d \'%s\' " FAIL "\n",          \
                   __FILE__, __LINE__, #x))

#define u_assert_var_equals(a, b)                                       \
    (((a) == (b))                                                         \
   ? (void)fprintf(stdout, "%s:%d %s(%ld) == %s(%ld) " PASS "\n",          \
                   __FILE__, __LINE__, #a, (long)a, #b, (long)b)        \
   : (void)fprintf(stdout, "%s:%d %s(%ld) == %s(%ld) " FAIL "\n",          \
                   __FILE__, __LINE__, #a, (long)a, #b, (long)b))

#define u_assert_ptr_equals(a, b)                                       \
    (((a) == (b))                                                         \
   ? (void)fprintf(stdout, "%s:%d %s(0x%08lx) == %s(0x%08lx) " PASS "\n",          \
                   __FILE__, __LINE__, #a, (long)a, #b, (long)b)        \
   : (void)fprintf(stdout, "%s:%d %s(0x%08lx) == %s(0x%08lx) " FAIL "\n",          \
                   __FILE__, __LINE__, #a, (long)a, #b, (long)b))

#endif
