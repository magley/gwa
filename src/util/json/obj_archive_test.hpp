#pragma once
#include "obj_archive.h"
#include "util/test/test2.h"

class Testing_P1 : ISerializable {
public:
    int x;
    float y;
    string2 name;

    Testing_P1() {}
    Testing_P1(int x, float y, string2 name) : x(x), y(y), name(name) {}

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

    bool operator==(const Testing_P1& other) const {
        return (x == other.x && y == other.y && name == other.name);
    }
    bool operator!=(const Testing_P1& other) const {
        return !(this->operator==(other));
    }
    
    friend std::ostream& operator<<(std::ostream& other, const Testing_P1& p) {
        other << "{" << p.x << ", " << p.y << ", " << p.name << "}";
        return other;
    }
};

class Testing_P2 : ISerializable {
public:
    int x;
    int y;

    Testing_P2() {}
    Testing_P2(int x, int y) : x(x), y(y) {}

    void save(ObjArchive& ar) const {
        ar.put("x", &x);
        ar.put("y", &y);
    }

    void load(const ObjArchive& ar) {
        ar.get("x", &x);
        ar.get("y", &y);
    }

    bool operator==(const Testing_P2& other) const {
        return (x == other.x && y == other.y);
    }
    bool operator!=(const Testing_P2& other) const {
        return !(this->operator==(other));
    }

    friend std::ostream& operator<<(std::ostream& other, const Testing_P2& p) {
        other << "{" << p.x << ", " << p.y << "}";
        return other;
    }
};

class Testing_P3 : ISerializable {
public:
    Testing_P2 pos;
    Testing_P2 vel;
    double angle;
    double rotspd;

    Testing_P3() {}

    void save(ObjArchive& ar) const {
        ar.put("pos", &pos);
        ar.put("vel", &vel);
        ar.put("angle", &angle);
        ar.put("rotspd", &rotspd);
    }

    void load(const ObjArchive& ar) {
        ar.get("pos", &pos);
        ar.get("vel", &vel);
        ar.get("angle", &angle);
        ar.get("rotspd", &rotspd);
    }

    bool operator==(const Testing_P3& other) const {
        return (pos == other.pos && vel == other.vel && angle == other.angle && rotspd == other.rotspd);
    }
    bool operator!=(const Testing_P3& other) const {
        return !(this->operator==(other));
    }

    friend std::ostream& operator<<(std::ostream& other, const Testing_P3& p) {
        other << "{" << p.pos << ", " << p.vel << ", " << p.angle << ", " << p.rotspd << "}";
        return other;
    }
};

class ObjArchive_Test : public test2::Test2 {
public:
    void run_tests() {
        EXECUTE_TEST(should_put_int);
        EXECUTE_TEST(should_put_long_long);
        EXECUTE_TEST(should_put_float);
        EXECUTE_TEST(should_put_double);
        EXECUTE_TEST(should_put_string2);
        EXECUTE_TEST(should_put_serializable);
        EXECUTE_TEST(should_put_vector);
        EXECUTE_TEST(should_push_various);
        EXECUTE_TEST(should_work_as_literal);
        EXECUTE_TEST(should_de_serialize_nested_structures);
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
        Testing_P1 p;
        p.x = 123;
        p.y = 0.1f;
        p.name = "John";

        ObjArchive ar;
        ar.put("myobj", &p);

        Testing_P1 p2;
        ar.get("myobj", &p2);

        ASSERT2_EQ(p.x, p2.x);
        ASSERT2_EQ(p.y, p2.y);
        ASSERT2_EQ(p.name, p2.name);
    }

    void should_put_vector() {
        std::vector<Testing_P1> vec;
        vec.push_back(Testing_P1(1, 0.1f, "John"));
        vec.push_back(Testing_P1(3, 0.2f, "Troy"));

        ObjArchive ar;
        ar.put("myarr", &vec);

        std::vector<Testing_P1> vec2;
        ar.get("myarr", &vec2);

        ASSERT2_EQ(vec, vec2);
    }

    void should_push_various() {
        ObjArchive ar;
        int x = 0;
        int y = 2;
        string2 z = "4";
        ar.push(&x);
        ar.push(&y);
        ar.push(&z);

        int xx, yy;
        string2 zz;
        ar.get(0, &xx);
        ar.get(1, &yy);
        ar.get(2, &zz);

        ASSERT2_EQ(x, xx);
        ASSERT2_EQ(y, yy);
        ASSERT2_EQ(z, zz);
    }

    void should_work_as_literal() {
        ObjArchive ar;

        int x = 123;
        ar.set(&x);

        int xx;
        ar.get(&xx);

        ASSERT2_EQ(x, xx);
    }

    void should_de_serialize_nested_structures() {
        Testing_P3 p3;
        p3.pos = Testing_P2(12, 12.3f);
        p3.vel = Testing_P2(-20, 0);
        p3.angle = 123.0;
        p3.rotspd = 12.0;

        Testing_P3 p3_clone;
        ObjArchive::from_str(ObjArchive::to_str(&p3), &p3_clone);

        ASSERT2_EQ(p3, p3_clone);
    }
};