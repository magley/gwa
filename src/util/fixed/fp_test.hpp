#include "fp.h"
#include "util/test/test2.h"

class fp6_Test : public test2::Test2 {
    void run_tests() {
        EXECUTE_TEST(addition);
    }

    void addition() {
        const fp6 f(10);
        const fp6 f_int = f + 1;
        const fp6 f_long = f + 1L;
        const fp6 f_longlong = f + 1LL;
        const fp6 f_float = f + 1.0f;
        const fp6 f_double = f + 1.0;
        const fp6 f_fp6 = f + fp6(1);
        const fp6 f_expect = fp6(11);
        
        ASSERT2_EQ(f_expect, f_int);
        ASSERT2_EQ(f_expect, f_long);
        ASSERT2_EQ(f_expect, f_longlong);
        ASSERT2_EQ(f_expect, f_float);
        ASSERT2_EQ(f_expect, f_double);
        ASSERT2_EQ(f_expect, f_fp6);
    }
};