#include "fp.h"
#include "util/test/test2.h"

class fp6_Test : public test2::Test2 {
    void run_tests() {
        EXECUTE_TEST(addition);
        EXECUTE_TEST(addition_other_side);
        EXECUTE_TEST(addition_assignment);
        EXECUTE_TEST(increment);

        EXECUTE_TEST(subtraction);
        EXECUTE_TEST(subtraction_other_side);
        EXECUTE_TEST(subtraction_assignment);
        EXECUTE_TEST(decrement);

        EXECUTE_TEST(multiplication);
        EXECUTE_TEST(multiplication_other_side);
        EXECUTE_TEST(multiplication_assignment);

        EXECUTE_TEST(division);
        EXECUTE_TEST(division_other_side);
        EXECUTE_TEST(division_assignment);
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

    void increment() {
        const fp6 f_expect = fp6(11);

        fp6 f1 = fp6(10);
        fp6 f1_inc_res = f1++;
        ASSERT2_EQ(fp6(10), f1_inc_res);
        ASSERT2_EQ(f_expect, f1);

        fp6 f2 = fp6(10);
        fp6 f2_inc_res = ++f2;
        ASSERT2_EQ(f_expect, f2_inc_res);
        ASSERT2_EQ(f_expect, f2);
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

    void decrement() {
        const fp6 f_expect = fp6(9);

        fp6 f1 = fp6(10);
        fp6 f1_inc_res = f1--;
        ASSERT2_EQ(fp6(10), f1_inc_res);
        ASSERT2_EQ(f_expect, f1);

        fp6 f2 = fp6(10);
        fp6 f2_inc_res = --f2;
        ASSERT2_EQ(f_expect, f2_inc_res);
        ASSERT2_EQ(f_expect, f2);
    }

    void multiplication() {
        const fp6 f(11);
        const fp6 f_expect = fp6(22);

        fp6 f_int = f * 2;
        fp6 f_long = f * 2L;
        fp6 f_longlong = f * 2LL;
        fp6 f_float = f * 2.0f;
        fp6 f_double = f * 2.0;
        fp6 f_fp6 = f * fp6(2);

        ASSERT2_EQ(f_expect, f_int);
        ASSERT2_EQ(f_expect, f_long);
        ASSERT2_EQ(f_expect, f_longlong);
        ASSERT2_EQ(f_expect, f_float);
        ASSERT2_EQ(f_expect, f_double);
        ASSERT2_EQ(f_expect, f_fp6);
    }

    void multiplication_other_side() {
        const fp6 f(11);
        const fp6 f_expect = fp6(22);

        fp6 f_int = 2 * f;
        fp6 f_long = 2L * f;
        fp6 f_longlong = 2LL * f;
        fp6 f_float = 2.0f * f;
        fp6 f_double = 2.0 * f;
        fp6 f_fp6 = fp6(2) * f; 

        ASSERT2_EQ(f_expect, f_int);
        ASSERT2_EQ(f_expect, f_long);
        ASSERT2_EQ(f_expect, f_longlong);
        ASSERT2_EQ(f_expect, f_float);
        ASSERT2_EQ(f_expect, f_double);
        ASSERT2_EQ(f_expect, f_fp6);
    }

    void multiplication_assignment() {
        fp6 f(10);
        const fp6 f_expect = fp6(30);

        f = 10; fp6 f_int = f *= 3;
        f = 10; fp6 f_long = f *= 3L;
        f = 10; fp6 f_longlong = f *= 3LL;
        f = 10; fp6 f_float = f *= 3.0f;
        f = 10; fp6 f_double = f *= 3.0;
        f = 10; fp6 f_fp6 = f *= fp6(3);

        ASSERT2_EQ(f_expect, f_int);
        ASSERT2_EQ(f_expect, f_long);
        ASSERT2_EQ(f_expect, f_longlong);
        ASSERT2_EQ(f_expect, f_float);
        ASSERT2_EQ(f_expect, f_double);
        ASSERT2_EQ(f_expect, f_fp6);
    }

    void division() {
        const fp6 f(22);
        const fp6 f_expect = fp6(2);

        fp6 f_int = f / 11;
        fp6 f_long = f / 11L;
        fp6 f_longlong = f / 11LL;
        fp6 f_float = f / 11.0f;
        fp6 f_double = f / 11.0;
        fp6 f_fp6 = f / fp6(11);

        ASSERT2_EQ(f_expect, f_int);
        ASSERT2_EQ(f_expect, f_long);
        ASSERT2_EQ(f_expect, f_longlong);
        ASSERT2_EQ(f_expect, f_float);
        ASSERT2_EQ(f_expect, f_double);
        ASSERT2_EQ(f_expect, f_fp6);
    }

    void division_other_side() {
        const fp6 f(6);
        const fp6 f_expect = fp6(2);

        fp6 f_int = 12 / f;
        fp6 f_long = 12L / f;
        fp6 f_longlong = 12LL / f;
        fp6 f_float = 12.0f / f;
        fp6 f_double = 12.0 / f;
        fp6 f_fp6 = fp6(12) / f; 

        ASSERT2_EQ(f_expect, f_int);
        ASSERT2_EQ(f_expect, f_long);
        ASSERT2_EQ(f_expect, f_longlong);
        ASSERT2_EQ(f_expect, f_float);
        ASSERT2_EQ(f_expect, f_double);
        ASSERT2_EQ(f_expect, f_fp6);
    }

    void division_assignment() {
        fp6 f(30);
        const fp6 f_expect = fp6(6);

        f = 30; fp6 f_int = f /= 5;
        f = 30; fp6 f_long = f /= 5L;
        f = 30; fp6 f_longlong = f /= 5LL;
        f = 30; fp6 f_float = f /= 5.0f;
        f = 30; fp6 f_double = f /= 5.0;
        f = 30; fp6 f_fp6 = f /= fp6(5);

        ASSERT2_EQ(f_expect, f_int);
        ASSERT2_EQ(f_expect, f_long);
        ASSERT2_EQ(f_expect, f_longlong);
        ASSERT2_EQ(f_expect, f_float);
        ASSERT2_EQ(f_expect, f_double);
        ASSERT2_EQ(f_expect, f_fp6);
    }
};