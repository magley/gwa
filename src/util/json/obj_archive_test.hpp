#pragma once
#include "obj_archive.h"
#include "util/test/test2.h"

struct ObjArchive_Test : public test2::Test2 {
    void run_tests() {
        EXECUTE_TEST(should_put_int);
        EXECUTE_TEST(should_put_long_long);
        EXECUTE_TEST(should_put_float);
        EXECUTE_TEST(should_put_double);
    }
private:
    void should_put_int() {
        int x = 1233321;
        ObjArchive ar;
        ar.put("x", &x);
        int x_back;
        ar.get("x", &x_back);

        ASSERT2_EQ(x, x_back);
    }

    void should_put_long_long() {
        long long x = (1LL << 32) + 123LL;
        ObjArchive ar;
        ar.put("x", &x);
        long long x_back;
        ar.get("x", &x_back);

        ASSERT2_EQ(x, x_back);
    }

    void should_put_float() {
        float x = 0.01234f;
        ObjArchive ar;
        ar.put("x", &x);
        float x_back;
        ar.get("x", &x_back);

        ASSERT2_EQ(x, x_back);
    }

    void should_put_double() {
        double x = 0.3;
        ObjArchive ar;
        ar.put("x", &x);
        double x_back;
        ar.get("x", &x_back);

        ASSERT2_EQ(x, x_back);
    }
};