#include "fp.h"
#include <iostream>
#include <math.h>

fp6::fp6(): _v(0) {}
fp6::fp6(int i): _v(i << _FP_N) {}
fp6::fp6(long i): _v(i << _FP_N) {}
fp6::fp6(long long i): _v(i << _FP_N) {}
fp6::fp6(float i): _v(roundf(i * (1 << _FP_N))) {}
fp6::fp6(double i): _v(round(i * (1 << _FP_N))) {}
fp6::fp6(const fp6& i): _v(i._v) {}

std::ostream& operator<<(std::ostream& other, const fp6& p) {
    other << (double)(p._v >> _FP_N);
    return other;
}

bool fp6::operator==(int i) const { return (_v >> _FP_N) == i; }
bool fp6::operator==(long i) const { return (_v >> _FP_N) == i; }
bool fp6::operator==(long long i) const { return (_v >> _FP_N) == i; }
bool fp6::operator==(float i) const { return _v == fp6(i)._v; }
bool fp6::operator==(double i) const { return _v == fp6(i)._v; }
bool fp6::operator==(const fp6& i) const { return _v == i._v; }

fp6 fp6::operator+(int i) const {
    return 0;
}
fp6 fp6::operator+(long i) const {
    return 0;
}
fp6 fp6::operator+(long long i) const {
    return 0;
}
fp6 fp6::operator+(float i) const {
    return 0;
}
fp6 fp6::operator+(double i) const {
    return 0;
}
fp6 fp6::operator+(const fp6& i) const {
    return 0;
}