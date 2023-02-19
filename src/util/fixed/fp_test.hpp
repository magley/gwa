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

        EXECUTE_TEST(assignment);

        EXECUTE_TEST(less_than);
        EXECUTE_TEST(less_than_left);
        EXECUTE_TEST(less_than_eq);
        EXECUTE_TEST(less_than_eq_left);

        EXECUTE_TEST(greater_than);
        EXECUTE_TEST(greater_than_left);
        EXECUTE_TEST(greater_than_eq);
        EXECUTE_TEST(greater_than_eq_left);

        EXECUTE_TEST(equals);
        EXECUTE_TEST(equals_left);
        EXECUTE_TEST(not_equals);
        EXECUTE_TEST(not_equals_left);

        EXECUTE_TEST(shift_left);
        EXECUTE_TEST(shift_right);

        EXECUTE_TEST(modulo);
        EXECUTE_TEST(modulo_left);
        EXECUTE_TEST(modulo_assignment);

        EXECUTE_TEST(conversion_operators);
        EXECUTE_TEST(unary_plus);
        EXECUTE_TEST(unary_minus);

        EXECUTE_TEST(biwise_not);
        EXECUTE_TEST(biwise_and);
        EXECUTE_TEST(biwise_or);
        EXECUTE_TEST(biwise_xor);
        EXECUTE_TEST(biwise_and_assign);
        EXECUTE_TEST(biwise_or_assign);
        EXECUTE_TEST(biwise_xor_assign);
        EXECUTE_TEST(bitwise_and_other_side);
        EXECUTE_TEST(bitwise_or_other_side);
        EXECUTE_TEST(bitwise_xor_other_side);
    }

    void addition() {
        const fp6 f(10);
        const fp6 f_expect = fp6(11);

        fp6 f_char = f + (char)1;
        fp6 f_short = f + (short)1;
        fp6 f_int = f + 1;
        fp6 f_long = f + 1L;
        fp6 f_longlong = f + 1LL;
        fp6 f_float = f + 1.0f;
        fp6 f_double = f + 1.0;
        fp6 f_fp6 = f + fp6(1);

        ASSERT2_EQ(f_expect, f_char);
        ASSERT2_EQ(f_expect, f_short);
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

        fp6 f_char = (char)1 + f;
        fp6 f_short = (short)1 + f;
        fp6 f_int = 1 + f;
        fp6 f_long = 1L + f;
        fp6 f_longlong = 1LL + f;
        fp6 f_float = 1.0f + f;
        fp6 f_double = 1.0 + f;
        fp6 f_fp6 = fp6(1) + f; 

        ASSERT2_EQ(f_expect, f_char);
        ASSERT2_EQ(f_expect, f_short);
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

        f = 10; fp6 f_char = f += (char)3;
        f = 10; fp6 f_short = f += (short)3;
        f = 10; fp6 f_int = f += 3;
        f = 10; fp6 f_long = f += 3L;
        f = 10; fp6 f_longlong = f += 3LL;
        f = 10; fp6 f_float = f += 3.0f;
        f = 10; fp6 f_double = f += 3.0;
        f = 10; fp6 f_fp6 = f += fp6(3);

        ASSERT2_EQ(f_expect, f_char);
        ASSERT2_EQ(f_expect, f_short);
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
        
        fp6 f_char = f - (char)1;
        fp6 f_short = f - (short)1;
        fp6 f_int = f - 1;
        fp6 f_long = f - 1L;
        fp6 f_longlong = f - 1LL;
        fp6 f_float = f - 1.0f;
        fp6 f_double = f - 1.0;
        fp6 f_fp6 = f - fp6(1);

        ASSERT2_EQ(f_expect, f_char);
        ASSERT2_EQ(f_expect, f_short);
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

        fp6 f_char = (char)1 - f;
        fp6 f_short = (short)1 - f;
        fp6 f_int = 1 - f;
        fp6 f_long = 1L - f;
        fp6 f_longlong = 1LL - f;
        fp6 f_float = 1.0f - f;
        fp6 f_double = 1.0 - f;
        fp6 f_fp6 = fp6(1) - f; 

        ASSERT2_EQ(f_expect, f_char);
        ASSERT2_EQ(f_expect, f_short);
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

        f = 13; fp6 f_char = f -= (char)3;
        f = 13; fp6 f_short = f -= (short)3;
        f = 13; fp6 f_int = f -= 3;
        f = 13; fp6 f_long = f -= 3L;
        f = 13; fp6 f_longlong = f -= 3LL;
        f = 13; fp6 f_float = f -= 3.0f;
        f = 13; fp6 f_double = f -= 3.0;
        f = 13; fp6 f_fp6 = f -= fp6(3);

        ASSERT2_EQ(f_expect, f_char);
        ASSERT2_EQ(f_expect, f_short);
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

        fp6 f_char = f * (char)2;
        fp6 f_short = f * (short)2;
        fp6 f_int = f * 2;
        fp6 f_long = f * 2L;
        fp6 f_longlong = f * 2LL;
        fp6 f_float = f * 2.0f;
        fp6 f_double = f * 2.0;
        fp6 f_fp6 = f * fp6(2);

        ASSERT2_EQ(f_expect, f_char);
        ASSERT2_EQ(f_expect, f_short);
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

        fp6 f_char = (char)2 * f;
        fp6 f_short = (short)2 * f;
        fp6 f_int = 2 * f;
        fp6 f_long = 2L * f;
        fp6 f_longlong = 2LL * f;
        fp6 f_float = 2.0f * f;
        fp6 f_double = 2.0 * f;
        fp6 f_fp6 = fp6(2) * f; 

        ASSERT2_EQ(f_expect, f_char);
        ASSERT2_EQ(f_expect, f_short);
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

        f = 10; fp6 f_char = f *= (char)3;
        f = 10; fp6 f_short = f *= (short)3;
        f = 10; fp6 f_int = f *= 3;
        f = 10; fp6 f_long = f *= 3L;
        f = 10; fp6 f_longlong = f *= 3LL;
        f = 10; fp6 f_float = f *= 3.0f;
        f = 10; fp6 f_double = f *= 3.0;
        f = 10; fp6 f_fp6 = f *= fp6(3);

        ASSERT2_EQ(f_expect, f_char);
        ASSERT2_EQ(f_expect, f_short);
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

        fp6 f_char = f / (char)11;
        fp6 f_short = f / (short)11;
        fp6 f_int = f / 11;
        fp6 f_long = f / 11L;
        fp6 f_longlong = f / 11LL;
        fp6 f_float = f / 11.0f;
        fp6 f_double = f / 11.0;
        fp6 f_fp6 = f / fp6(11);

        ASSERT2_EQ(f_expect, f_char);
        ASSERT2_EQ(f_expect, f_short);
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

        fp6 f_char = (char)12 / f;
        fp6 f_short = (short)12 / f;
        fp6 f_int = 12 / f;
        fp6 f_long = 12L / f;
        fp6 f_longlong = 12LL / f;
        fp6 f_float = 12.0f / f;
        fp6 f_double = 12.0 / f;
        fp6 f_fp6 = fp6(12) / f;

        ASSERT2_EQ(f_expect, f_char);
        ASSERT2_EQ(f_expect, f_short);
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

        f = 30; fp6 f_char = f /= (char)5;
        f = 30; fp6 f_short = f /= (short)5;
        f = 30; fp6 f_int = f /= 5;
        f = 30; fp6 f_long = f /= 5L;
        f = 30; fp6 f_longlong = f /= 5LL;
        f = 30; fp6 f_float = f /= 5.0f;
        f = 30; fp6 f_double = f /= 5.0;
        f = 30; fp6 f_fp6 = f /= fp6(5);

        ASSERT2_EQ(f_expect, f_char);
        ASSERT2_EQ(f_expect, f_short);
        ASSERT2_EQ(f_expect, f_int);
        ASSERT2_EQ(f_expect, f_long);
        ASSERT2_EQ(f_expect, f_longlong);
        ASSERT2_EQ(f_expect, f_float);
        ASSERT2_EQ(f_expect, f_double);
        ASSERT2_EQ(f_expect, f_fp6);
    }

    void assignment() {
        fp6 f(0);

        f = (char)1;
        ASSERT2_EQ(fp6((char)1), f);

        f = (short)1;
        ASSERT2_EQ(fp6((short)1), f);

        f = 1;
        ASSERT2_EQ(fp6(1), f);

        f = 2L;
        ASSERT2_EQ(fp6(2L), f);

        f = 3LL;
        ASSERT2_EQ(fp6(3LL), f);

        f = 4.5f;
        ASSERT2_EQ(fp6(4.5f), f);

        f = 6.78;
        ASSERT2_EQ(fp6(6.78), f);

        f = fp6(9);
        ASSERT2_EQ(fp6(9), f);
    }

    void less_than() {
        fp6 f1(1);

        ASSERT2(f1 < (char)2);
        ASSERT2(f1 < (short)2);
        ASSERT2(f1 < 2);
        ASSERT2(f1 < 2L);
        ASSERT2(f1 < 2LL);
        ASSERT2(f1 < 2.0f);
        ASSERT2(f1 < 2.0);
        ASSERT2(f1 < fp6(2));
    }

    void less_than_left() {
        fp6 f1(10);

        ASSERT2((char)2 < f1);
        ASSERT2((short)2 < f1);
        ASSERT2(2 < f1);
        ASSERT2(2L < f1);
        ASSERT2(2LL < f1);
        ASSERT2(2.0f < f1);
        ASSERT2(2.0 < f1);
        ASSERT2(fp6(2) < f1);
    }

    void less_than_eq() {
        fp6 f1(1);

        ASSERT2(f1 <= (char)2);
        ASSERT2(f1 <= (short)2);
        ASSERT2(f1 <= 2);
        ASSERT2(f1 <= 2L);
        ASSERT2(f1 <= 2LL);
        ASSERT2(f1 <= 2.0f);
        ASSERT2(f1 <= 2.0);
        ASSERT2(f1 <= fp6(2));

        f1 = fp6(2);
        ASSERT2(f1 <= (char)2);
        ASSERT2(f1 <= (short)2);
        ASSERT2(f1 <= 2);
        ASSERT2(f1 <= 2L);
        ASSERT2(f1 <= 2LL);
        ASSERT2(f1 <= 2.0f);
        ASSERT2(f1 <= 2.0);
        ASSERT2(f1 <= fp6(2));
    }

    void less_than_eq_left() {
        fp6 f1(10);

        ASSERT2((char)2 <= f1);
        ASSERT2((short)2 <= f1);
        ASSERT2(2 <= f1);
        ASSERT2(2L <= f1);
        ASSERT2(2LL <= f1);
        ASSERT2(2.0f <= f1);
        ASSERT2(2.0 <= f1);
        ASSERT2(fp6(2) <= f1);

        f1 = fp6(2);
        ASSERT2((char)2 <= f1);
        ASSERT2((short)2 <= f1);
        ASSERT2(2 <= f1);
        ASSERT2(2L <= f1);
        ASSERT2(2LL <= f1);
        ASSERT2(2.0f <= f1);
        ASSERT2(2.0 <= f1);
        ASSERT2(fp6(2) <= f1);
    }

    void greater_than() {
        fp6 f1(4);

        ASSERT2(f1 > (char)2);
        ASSERT2(f1 > (short)2);
        ASSERT2(f1 > 2);
        ASSERT2(f1 > 2L);
        ASSERT2(f1 > 2LL);
        ASSERT2(f1 > 2.0f);
        ASSERT2(f1 > 2.0);
        ASSERT2(f1 > fp6(2));
    }

    void greater_than_left() {
        fp6 f1(1);

        ASSERT2((char)2 > f1);
        ASSERT2((short)2 > f1);
        ASSERT2(2 > f1);
        ASSERT2(2L > f1);
        ASSERT2(2LL > f1);
        ASSERT2(2.0f > f1);
        ASSERT2(2.0 > f1);
        ASSERT2(fp6(2) > f1);
    }

    void greater_than_eq() {
        fp6 f1(4);

        ASSERT2(f1 >= (char)2);
        ASSERT2(f1 >= (short)2);
        ASSERT2(f1 >= 2);
        ASSERT2(f1 >= 2L);
        ASSERT2(f1 >= 2LL);
        ASSERT2(f1 >= 2.0f);
        ASSERT2(f1 >= 2.0);
        ASSERT2(f1 >= fp6(2));

        f1 = fp6(2);
        ASSERT2(f1 >= (char)2);
        ASSERT2(f1 >= (short)2);
        ASSERT2(f1 >= 2);
        ASSERT2(f1 >= 2L);
        ASSERT2(f1 >= 2LL);
        ASSERT2(f1 >= 2.0f);
        ASSERT2(f1 >= 2.0);
        ASSERT2(f1 >= fp6(2));
    }

    void greater_than_eq_left() {
        fp6 f1(1);

        ASSERT2((char)2 >= f1);
        ASSERT2((short)2 >= f1);
        ASSERT2(2 >= f1);
        ASSERT2(2L >= f1);
        ASSERT2(2LL >= f1);
        ASSERT2(2.0f >= f1);
        ASSERT2(2.0 >= f1);
        ASSERT2(fp6(2) >= f1);

        f1 = fp6(2);
        ASSERT2((char)2 >= f1);
        ASSERT2((short)2 >= f1);
        ASSERT2(2 >= f1);
        ASSERT2(2L >= f1);
        ASSERT2(2LL >= f1);
        ASSERT2(2.0f >= f1);
        ASSERT2(2.0 >= f1);
        ASSERT2(fp6(2) >= f1);
    }

    void equals() {
        fp6 f1(2);

        ASSERT2(f1 == (char)2);
        ASSERT2(f1 == (short)2);
        ASSERT2(f1 == 2);
        ASSERT2(f1 == 2L);
        ASSERT2(f1 == 2LL);
        ASSERT2(f1 == 2.0f);
        ASSERT2(f1 == 2.0);
        ASSERT2(f1 == fp6(2));
    }

    void equals_left() {
        fp6 f1(2);

        ASSERT2((char)2 == f1);
        ASSERT2((short)2 == f1);
        ASSERT2(2 == f1);
        ASSERT2(2L == f1);
        ASSERT2(2LL == f1);
        ASSERT2(2.0f == f1);
        ASSERT2(2.0 == f1);
        ASSERT2(fp6(2) == f1);
    }

    void not_equals() {
        fp6 f1(3);

        ASSERT2(f1 != (char)2);
        ASSERT2(f1 != (short)2);
        ASSERT2(f1 != 2);
        ASSERT2(f1 != 2L);
        ASSERT2(f1 != 2LL);
        ASSERT2(f1 != 2.0f);
        ASSERT2(f1 != 2.0);
        ASSERT2(f1 != fp6(2));
    }

    void not_equals_left() {
        fp6 f1(3);

        ASSERT2((char)2 != f1);
        ASSERT2((short)2 != f1);
        ASSERT2(2 != f1);
        ASSERT2(2L != f1);
        ASSERT2(2LL != f1);
        ASSERT2(2.0f != f1);
        ASSERT2(2.0 != f1);
        ASSERT2(fp6(2) != f1);
    }

    void shift_left() {
        fp6 f1(1);
        fp6 f2(1 << 4);

        ASSERT2_EQ(f2, f1 << 4);

        f1 <<= 4;
        ASSERT2_EQ(f2, f1);
    }

    void shift_right() {
        fp6 f1(65536);
        fp6 f2(65536 >> 4);

        ASSERT2_EQ(f2, f1 >> 4);

        f1 >>= 4;
        ASSERT2_EQ(f2, f1);
    }

    void modulo() {
        fp6 f(5);

        for (int i = 0; i < 5; i++) {
            ASSERT2_EQ(fp6(5 % (i + 1)), f % (i + (char)1));
            ASSERT2_EQ(fp6(5 % (i + 1)), f % (i + (short)1));
            ASSERT2_EQ(fp6(5 % (i + 1)), f % (i + 1));
            ASSERT2_EQ(fp6(5 % (i + 1)), f % (i + 1L));
            ASSERT2_EQ(fp6(5 % (i + 1)), f % (i + 1LL));
            ASSERT2_EQ(fp6(5 % (i + 1)), f % fp6(i + 1));
        }
    }

    void modulo_left() {
        for (int i = 0; i < 5; i++) {
            ASSERT2_EQ(fp6(5 % (i + 1)), (char)5 % fp6(i + 1));
            ASSERT2_EQ(fp6(5 % (i + 1)), (short)5 % fp6(i + 1));         
            ASSERT2_EQ(fp6(5 % (i + 1)), 5 % fp6(i + 1));
            ASSERT2_EQ(fp6(5 % (i + 1)), 5L % fp6(i + 1));
            ASSERT2_EQ(fp6(5 % (i + 1)), 5LL % fp6(i + 1));
        }
    }

    void modulo_assignment() {
        fp6 f(5);

        for (int i = 0; i < 5; i++) {
            ASSERT2_EQ(fp6(5 % (i + 1)), f %= (char)(i + (char)1)); f = fp6(5);
            ASSERT2_EQ(fp6(5 % (i + 1)), f %= (short)(i + (short)1)); f = fp6(5);
            ASSERT2_EQ(fp6(5 % (i + 1)), f %= (i + 1)); f = fp6(5);
            ASSERT2_EQ(fp6(5 % (i + 1)), f %= (i + 1L)); f = fp6(5);
            ASSERT2_EQ(fp6(5 % (i + 1)), f %= (i + 1LL)); f = fp6(5);
            ASSERT2_EQ(fp6(5 % (i + 1)), f %= fp6(i + 1)); f = fp6(5);
        }
    }

    void conversion_operators() {
        fp6 f(10.5);
        char v00 = (char)f;
        short v0 = (short)f;
        int v1 = (int)f;
        long v2 = (long)f;
        long long v3 = (long long)f;
        float v4 = (float)f;
        double v5 = (double)f;

        ASSERT2_EQ((char)10, v00);
        ASSERT2_EQ((short)10, v0);
        ASSERT2_EQ(10, v1);
        ASSERT2_EQ(10L, v2);
        ASSERT2_EQ(10LL, v3);
        ASSERT2_EQ(10.5f, v4);
        ASSERT2_EQ(10.5, v5);
    }

    void unary_plus() {
        fp6 f(5);
        ASSERT2_EQ(f, +f);
    }

    void unary_minus() {
        fp6 f(5);
        ASSERT2_EQ(f * -1, -f);
    }

    void biwise_not() {
        fp6 f(0);
        ASSERT2_EQ(fp6(~0), ~f);
    }

    void biwise_and() {
        fp6 f(7);
        ASSERT2_EQ(fp6(7 & 3), f & 3);
        ASSERT2_EQ(fp6(7 & 3), f & fp6(3));
    }

    void biwise_or() {
        fp6 f(7);
        ASSERT2_EQ(fp6(7 | 3), f | 3);
        ASSERT2_EQ(fp6(7 | 3), f | fp6(3));
    }

    void biwise_xor() {
        fp6 f(7);
        ASSERT2_EQ(fp6(7 ^ 3), f ^ 3);
        ASSERT2_EQ(fp6(7 ^ 3), f ^ fp6(3));
    }

    void biwise_and_assign() {
        fp6 f(7);
        ASSERT2_EQ(fp6(7 & 3), f &= 3); f = 7;
        ASSERT2_EQ(fp6(7 & 3), f &= fp6(3)); f = 7;
    }

    void biwise_or_assign() {
        fp6 f(7);
        ASSERT2_EQ(fp6(7 | 3), f |= 3); f = 7;
        ASSERT2_EQ(fp6(7 | 3), f |= fp6(3)); f = 7;
    }

    void biwise_xor_assign() {
        fp6 f(7);
        ASSERT2_EQ(fp6(7 ^ 3), f ^= 3); f = 7;
        ASSERT2_EQ(fp6(7 ^ 3), f ^= fp6(3)); f = 7;
    }

    void bitwise_and_other_side() {
        int i = 7;
        ASSERT2_EQ(fp6(7 & 3), i & fp6(3));
        ASSERT2_EQ(7 & 3, i &= fp6(3));
    }

    void bitwise_or_other_side() {
        int i = 7;
        ASSERT2_EQ(fp6(7 | 3), i | fp6(3));
        ASSERT2_EQ(7 | 3, i |= fp6(3));
    }

    void bitwise_xor_other_side() {
        int i = 7;
        ASSERT2_EQ(fp6(7 ^ 3), i ^ fp6(3));
        ASSERT2_EQ(7 ^ 3, i ^= fp6(3));
    }
};