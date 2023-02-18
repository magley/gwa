#pragma once

#include <stdint.h>
#include <iosfwd>

#define _FP_N 6

class fp6 {
public:
    int32_t _v;
    fp6();
    fp6(int i);
    fp6(long i);
    fp6(long long i);
    fp6(float i);
    fp6(double i);
    fp6(const fp6& i);
    inline static fp6 raw(int raw_val);

    friend std::ostream& operator<<(std::ostream& other, const fp6& p);

    bool operator==(int i) const;
    bool operator==(long i) const;
    bool operator==(long long i) const;
    bool operator==(float i) const;
    bool operator==(double i) const;
    bool operator==(const fp6& i) const;

    fp6 operator+(int i) const;
    fp6 operator+(long i) const;
    fp6 operator+(long long i) const;
    fp6 operator+(float i) const;
    fp6 operator+(double i) const;
    fp6 operator+(const fp6& i) const;

    fp6& operator+=(int i);
    fp6& operator+=(long i);
    fp6& operator+=(long long i);
    fp6& operator+=(float i);
    fp6& operator+=(double i);
    fp6& operator+=(const fp6& i);

    fp6& operator++();
    fp6 operator++(int);

    fp6 operator-(int i) const;
    fp6 operator-(long i) const;
    fp6 operator-(long long i) const;
    fp6 operator-(float i) const;
    fp6 operator-(double i) const;
    fp6 operator-(const fp6& i) const;

    fp6& operator-=(int i);
    fp6& operator-=(long i);
    fp6& operator-=(long long i);
    fp6& operator-=(float i);
    fp6& operator-=(double i);
    fp6& operator-=(const fp6& i);

    fp6& operator--();
    fp6 operator--(int);
};

fp6 operator+(int i, const fp6& f);
fp6 operator+(long i, const fp6& f);
fp6 operator+(long long i, const fp6& f);
fp6 operator+(float i, const fp6& f);
fp6 operator+(double i, const fp6& f);

fp6 operator-(int i, const fp6& f);
fp6 operator-(long i, const fp6& f);
fp6 operator-(long long i, const fp6& f);
fp6 operator-(float i, const fp6& f);
fp6 operator-(double i, const fp6& f);