#include "fp.h"
#include "util/test/test2.h"

class fp6_Test : public test2::Test2 {
    void run_tests() {
        EXECUTE_TEST(addition);
        EXECUTE_TEST(addition_other_side);
        EXECUTE_TEST(addition_assignment);

        EXECUTE_TEST(subtraction);
        EXECUTE_TEST(subtraction_other_side);
        EXECUTE_TEST(subtraction_assignment);
    }

    void addition() {
        const fp6 f(10);
        const fp6 f_expect = fp6(11);

        fp6 f_int = f + 1;
        fp6 f_long = f + 1L;
        fp6 f_longlong = f + 1LL;
        fp6 f_float = f + 1.0f;
        fp6 f_double = f + 1.0;
        fp6 f_fp6 = f + fp6(1);

        ASSERT2_EQ(f_expect, f_int);
        ASSERT2_EQ(f_expect, f_long);
        ASSERT2_EQ(f_expect, f_longlong);
        ASSERT2_EQ(f_expect, f_float);
        ASSERT2_EQ(f_expect, f_double);
        ASSERT2_EQ(f_expect, f_fp6);
    }

    void addition_other_side() {
        const fp6 f(10);
        const fp6 f_expect = fp6(11);

        fp6 f_int = 1 + f;
        fp6 f_long = 1L + f;
        fp6 f_longlong = 1LL + f;
        fp6 f_float = 1.0f + f;
        fp6 f_double = 1.0 + f;
        fp6 f_fp6 = fp6(1) + f; 

        ASSERT2_EQ(f_expect, f_int);
        ASSERT2_EQ(f_expect, f_long);
        ASSERT2_EQ(f_expect, f_longlong);
        ASSERT2_EQ(f_expect, f_float);
        ASSERT2_EQ(f_expect, f_double);
        ASSERT2_EQ(f_expect, f_fp6);
    }

    void addition_assignment() {
        fp6 f(10);
        const fp6 f_expect = fp6(13);

        f = 10; fp6 f_int = f += 3;
        f = 10; fp6 f_long = f += 3L;
        f = 10; fp6 f_longlong = f += 3LL;
        f = 10; fp6 f_float = f += 3.0f;
        f = 10; fp6 f_double = f += 3.0;
        f = 10; fp6 f_fp6 = f += fp6(3);

        ASSERT2_EQ(f_expect, f_int);
        ASSERT2_EQ(f_expect, f_long);
        ASSERT2_EQ(f_expect, f_longlong);
        ASSERT2_EQ(f_expect, f_float);
        ASSERT2_EQ(f_expect, f_double);
        ASSERT2_EQ(f_expect, f_fp6);
    }

    void subtraction() {
        const fp6 f(11);
        const fp6 f_expect = fp6(10);

        fp6 f_int = f - 1;
        fp6 f_long = f - 1L;
        fp6 f_longlong = f - 1LL;
        fp6 f_float = f - 1.0f;
        fp6 f_double = f - 1.0;
        fp6 f_fp6 = f - fp6(1);

        ASSERT2_EQ(f_expect, f_int);
        ASSERT2_EQ(f_expect, f_long);
        ASSERT2_EQ(f_expect, f_longlong);
        ASSERT2_EQ(f_expect, f_float);
        ASSERT2_EQ(f_expect, f_double);
        ASSERT2_EQ(f_expect, f_fp6);
    }

    void subtraction_other_side() {
        const fp6 f(11);
        const fp6 f_expect = fp6(-10);

        fp6 f_int = 1 - f;
        fp6 f_long = 1L - f;
        fp6 f_longlong = 1LL - f;
        fp6 f_float = 1.0f - f;
        fp6 f_double = 1.0 - f;
        fp6 f_fp6 = fp6(1) - f; 

        ASSERT2_EQ(f_expect, f_int);
        ASSERT2_EQ(f_expect, f_long);
        ASSERT2_EQ(f_expect, f_longlong);
        ASSERT2_EQ(f_expect, f_float);
        ASSERT2_EQ(f_expect, f_double);
        ASSERT2_EQ(f_expect, f_fp6);
    }

    void subtraction_assignment() {
        fp6 f(13);
        const fp6 f_expect = fp6(10);

        f = 13; fp6 f_int = f -= 3;
        f = 13; fp6 f_long = f -= 3L;
        f = 13; fp6 f_longlong = f -= 3LL;
        f = 13; fp6 f_float = f -= 3.0f;
        f = 13; fp6 f_double = f -= 3.0;
        f = 13; fp6 f_fp6 = f -= fp6(3);

        ASSERT2_EQ(f_expect, f_int);
        ASSERT2_EQ(f_expect, f_long);
        ASSERT2_EQ(f_expect, f_longlong);
        ASSERT2_EQ(f_expect, f_float);
        ASSERT2_EQ(f_expect, f_double);
        ASSERT2_EQ(f_expect, f_fp6);
    }
};