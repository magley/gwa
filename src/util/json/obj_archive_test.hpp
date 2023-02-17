#pragma once
#include "obj_archive.h"
#include "util/test/test2.h"

struct Testing_P2 : ISerializable {
    int x;
    float y;
    string2 name;

    Testing_P2() {}
    Testing_P2(int x, float y, string2 name) : x(x), y(y), name(name) {}

    void save(ObjArchive& ar) const {
        ar.put("x", &x);
        ar.put("y", &y);
        ar.put("name", &name);
    }

    void load(const ObjArchive& ar) {
        ar.get("x", &x);
        ar.get("y", &y);
        ar.get("name", &name);
    }

    bool operator==(const Testing_P2& other) const {
        return (x == other.x && y == other.y && name == other.name);
    }
    bool operator!=(const Testing_P2& other) const {
        return !(x == other.x && y == other.y && name == other.name);
    }
    
    friend std::ostream& operator<<(std::ostream& other, const Testing_P2& p) {
        other << "{" << p.x << ", " << p.y << ", " << p.name << "}";
        return other;
    }
};

struct ObjArchive_Test : public test2::Test2 {
    void run_tests() {
        EXECUTE_TEST(should_put_int);
        EXECUTE_TEST(should_put_long_long);
        EXECUTE_TEST(should_put_float);
        EXECUTE_TEST(should_put_double);
        EXECUTE_TEST(should_put_string2);
        EXECUTE_TEST(should_put_serializable);
        EXECUTE_TEST(should_put_vector);
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

    void should_put_string2() {
        string2 s = "Hello world";
        ObjArchive ar;
        ar.put("name", &s);
        string2 s2;
        ar.get("name", &s2);

        ASSERT2_EQ(s, s2);
    }

    void should_put_serializable() {
        Testing_P2 p;
        p.x = 123;
        p.y = 0.1f;
        p.name = "John";

        ObjArchive ar;
        ar.put("myobj", &p);

        Testing_P2 p2;
        ar.get("myobj", &p2);

        ASSERT2_EQ(p.x, p2.x);
        ASSERT2_EQ(p.y, p2.y);
        ASSERT2_EQ(p.name, p2.name);
    }

    void should_put_vector() {
        std::vector<Testing_P2> vec;
        vec.push_back(Testing_P2(1, 0.1f, "John"));
        vec.push_back(Testing_P2(3, 0.2f, "Troy"));

        ObjArchive ar;
        ar.put("myarr", &vec);

        std::vector<Testing_P2> vec2;
        ar.get("myarr", &vec2);

        ASSERT2_EQ(vec, vec2);
    }
};