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
inline fp6 fp6::raw(int raw_val) { fp6 r; r._v = raw_val; return r; }

std::ostream& operator<<(std::ostream& other, const fp6& p) {
    other << (double)(p._v / (double)(1 << _FP_N));
    return other;
}

std::istream& operator>>(std::istream& other, fp6& p) {
    double d;
    other >> d;
    p = fp6(d);
    return other;
}

bool fp6::operator==(int i) const { return (_v >> _FP_N) == i; }
bool fp6::operator==(long i) const { return (_v >> _FP_N) == i; }
bool fp6::operator==(long long i) const { return (_v >> _FP_N) == i; }
bool fp6::operator==(float i) const { return _v == fp6(i)._v; }
bool fp6::operator==(double i) const { return _v == fp6(i)._v; }
bool fp6::operator==(const fp6& i) const { return _v == i._v; }

bool operator==(int i, const fp6& f) { return (f._v >> _FP_N) == i; }
bool operator==(long i, const fp6& f) { return (f._v >> _FP_N) == i; }
bool operator==(long long i, const fp6& f) { return (f._v >> _FP_N) == i; }
bool operator==(float i, const fp6& f) { return f._v == fp6(i)._v; }
bool operator==(double i, const fp6& f) { return f._v == fp6(i)._v; }

bool fp6::operator!=(int i) const { return (_v >> _FP_N) != i; }
bool fp6::operator!=(long i) const { return (_v >> _FP_N) != i; }
bool fp6::operator!=(long long i) const { return (_v >> _FP_N) != i; }
bool fp6::operator!=(float i) const { return _v != fp6(i)._v; }
bool fp6::operator!=(double i) const { return _v != fp6(i)._v; }
bool fp6::operator!=(const fp6& i) const { return _v != i._v; }

bool operator!=(int i, const fp6& f) { return (f._v >> _FP_N) != i; }
bool operator!=(long i, const fp6& f) { return (f._v >> _FP_N) != i; }
bool operator!=(long long i, const fp6& f) { return (f._v >> _FP_N) != i; }
bool operator!=(float i, const fp6& f) { return f._v != fp6(i)._v; }
bool operator!=(double i, const fp6& f) { return f._v != fp6(i)._v; }

bool fp6::operator<(int i) const { return _v < (i << _FP_N); }
bool fp6::operator<(long i) const { return _v < (i << _FP_N); }
bool fp6::operator<(long long i) const { return _v < (i << _FP_N); }
bool fp6::operator<(float i) const { return _v < roundf(i * (1 << _FP_N)); }
bool fp6::operator<(double i) const { return _v < round(i * (1 << _FP_N)); }
bool fp6::operator<(const fp6& i) const { return _v < i._v; }

bool operator<(int i, const fp6& f) { return (i << _FP_N) < f._v; }
bool operator<(long i, const fp6& f) { return (i << _FP_N) < f._v; }
bool operator<(long long i, const fp6& f) { return (i << _FP_N) < f._v; }
bool operator<(float i, const fp6& f) { return (roundf(i * (1 << _FP_N))) < f._v; }
bool operator<(double i, const fp6& f) { return (round(i * (1 << _FP_N))) < f._v; }

bool fp6::operator>(int i) const { return _v > (i << _FP_N); }
bool fp6::operator>(long i) const { return _v > (i << _FP_N); }
bool fp6::operator>(long long i) const { return _v > (i << _FP_N); }
bool fp6::operator>(float i) const { return _v > roundf(i * (1 << _FP_N)); }
bool fp6::operator>(double i) const { return _v > round(i * (1 << _FP_N)); }
bool fp6::operator>(const fp6& i) const { return _v > i._v; }

bool operator>(int i, const fp6& f) { return (i << _FP_N) > f._v; }
bool operator>(long i, const fp6& f) { return (i << _FP_N) > f._v; }
bool operator>(long long i, const fp6& f) { return (i << _FP_N) > f._v; }
bool operator>(float i, const fp6& f) { return (roundf(i * (1 << _FP_N))) > f._v; }
bool operator>(double i, const fp6& f) { return (round(i * (1 << _FP_N))) > f._v; }

bool fp6::operator<=(int i) const { return _v <= (i << _FP_N); }
bool fp6::operator<=(long i) const { return _v <= (i << _FP_N); }
bool fp6::operator<=(long long i) const { return _v <= (i << _FP_N); }
bool fp6::operator<=(float i) const { return _v <= roundf(i * (1 << _FP_N)); }
bool fp6::operator<=(double i) const { return _v <= round(i * (1 << _FP_N)); }
bool fp6::operator<=(const fp6& i) const { return _v <= i._v; }

bool operator<=(int i, const fp6& f) { return (i << _FP_N) <= f._v; }
bool operator<=(long i, const fp6& f) { return (i << _FP_N) <= f._v; }
bool operator<=(long long i, const fp6& f) { return (i << _FP_N) <= f._v; }
bool operator<=(float i, const fp6& f) { return (roundf(i * (1 << _FP_N))) <= f._v; }
bool operator<=(double i, const fp6& f) { return (round(i * (1 << _FP_N))) <= f._v; }

bool fp6::operator>=(int i) const { return _v >= (i << _FP_N); }
bool fp6::operator>=(long i) const { return _v >= (i << _FP_N); }
bool fp6::operator>=(long long i) const { return _v >= (i << _FP_N); }
bool fp6::operator>=(float i) const { return _v >= roundf(i * (1 << _FP_N)); }
bool fp6::operator>=(double i) const { return _v >= round(i * (1 << _FP_N)); }
bool fp6::operator>=(const fp6& i) const { return _v >= i._v; }

bool operator>=(int i, const fp6& f) { return (i << _FP_N) >= f._v; }
bool operator>=(long i, const fp6& f) { return (i << _FP_N) >= f._v; }
bool operator>=(long long i, const fp6& f) { return (i << _FP_N) >= f._v; }
bool operator>=(float i, const fp6& f) { return (roundf(i * (1 << _FP_N))) >= f._v; }
bool operator>=(double i, const fp6& f) { return (round(i * (1 << _FP_N))) >= f._v; }

fp6& fp6::operator=(int i) {
    _v = i << _FP_N;
    return *this;
}
fp6& fp6::operator=(long i) {
    _v = i << _FP_N;
    return *this;
}
fp6& fp6::operator=(long long i) {
    _v = i << _FP_N;
    return *this;
}
fp6& fp6::operator=(float i) {
    _v = roundf(i * (1 << _FP_N));
    return *this;
}
fp6& fp6::operator=(double i) {
    _v = round(i * (1 << _FP_N));
    return *this;
}
fp6& fp6::operator=(const fp6& i) {
    _v = i._v;
    return *this;
}

fp6 fp6::operator+(int i) const {
    return raw(_v + (i << _FP_N));
}
fp6 fp6::operator+(long i) const {
    return raw(_v + (i << _FP_N));
}
fp6 fp6::operator+(long long i) const {
    return raw(_v + (i << _FP_N));
}
fp6 fp6::operator+(float i) const {
    return raw(_v + (roundf(i * (1 << _FP_N))));
}
fp6 fp6::operator+(double i) const {
    return raw(_v + (round(i * (1 << _FP_N))));
}
fp6 fp6::operator+(const fp6& i) const {
    return raw(_v + i._v);
}

fp6 operator+(int i, const fp6& f) { 
    return fp6::raw((i << _FP_N) + f._v); 
}
fp6 operator+(long i, const fp6& f) { 
    return fp6::raw((i << _FP_N) + f._v); 
}
fp6 operator+(long long i, const fp6& f) { 
    return fp6::raw((i << _FP_N) + f._v); 
}
fp6 operator+(float i, const fp6& f) { 
    return fp6::raw((roundf(i * (1 << _FP_N))) + f._v);
}
fp6 operator+(double i, const fp6& f) { 
    return fp6::raw((round(i * (1 << _FP_N))) + f._v);
}

fp6& fp6::operator+=(int i) {
    _v += (i << _FP_N);
    return *this;
}
fp6& fp6::operator+=(long i) {
    _v += (i << _FP_N);
    return *this;
}
fp6& fp6::operator+=(long long i) {
    _v += (i << _FP_N);
    return *this;
}
fp6& fp6::operator+=(float i) {
    _v += roundf(i * (1 << _FP_N));
    return *this;
}
fp6& fp6::operator+=(double i) {
    _v += round(i * (1 << _FP_N));
    return *this;
}
fp6& fp6::operator+=(const fp6& i) {
    _v += i._v;
    return *this;
}

fp6& fp6::operator++() {
    _v += (1 << _FP_N);
    return *this;
}

fp6 fp6::operator++(int) {
    fp6 before = raw(_v);
    _v += (1 << _FP_N);
    return before;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////

fp6 fp6::operator-(int i) const {
    return raw(_v - (i << _FP_N));
}
fp6 fp6::operator-(long i) const {
    return raw(_v - (i << _FP_N));
}
fp6 fp6::operator-(long long i) const {
    return raw(_v - (i << _FP_N));
}
fp6 fp6::operator-(float i) const {
    return raw(_v - (roundf(i * (1 << _FP_N))));
}
fp6 fp6::operator-(double i) const {
    return raw(_v - (round(i * (1 << _FP_N))));
}
fp6 fp6::operator-(const fp6& i) const {
    return raw(_v - i._v);
}

fp6 operator-(int i, const fp6& f) { 
    return fp6::raw((i << _FP_N) - f._v); 
}
fp6 operator-(long i, const fp6& f) { 
    return fp6::raw((i << _FP_N) - f._v); 
}
fp6 operator-(long long i, const fp6& f) { 
    return fp6::raw((i << _FP_N) - f._v); 
}
fp6 operator-(float i, const fp6& f) { 
    return fp6::raw((roundf(i * (1 << _FP_N))) - f._v);
}
fp6 operator-(double i, const fp6& f) { 
    return fp6::raw((round(i * (1 << _FP_N))) - f._v);
}

fp6& fp6::operator-=(int i) {
    _v -= (i << _FP_N);
    return *this;
}
fp6& fp6::operator-=(long i) {
    _v -= (i << _FP_N);
    return *this;
}
fp6& fp6::operator-=(long long i) {
    _v -= (i << _FP_N);
    return *this;
}
fp6& fp6::operator-=(float i) {
    _v -= roundf(i * (1 << _FP_N));
    return *this;
}
fp6& fp6::operator-=(double i) {
    _v -= round(i * (1 << _FP_N));
    return *this;
}
fp6& fp6::operator-=(const fp6& i) {
    _v -= i._v;
    return *this;
}

fp6& fp6::operator--() {
    _v -= (1 << _FP_N);
    return *this;
}

fp6 fp6::operator--(int) {
    fp6 before = raw(_v);
    _v -= (1 << _FP_N);
    return before;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////

fp6 fp6::operator*(int i) const {
    return raw(_v * i);
}
fp6 fp6::operator*(long i) const {
    return raw(_v * i);
}
fp6 fp6::operator*(long long i) const {
    return raw(_v * i);
}
fp6 fp6::operator*(float i) const {
    return raw((_v * (int)roundf(i * (1 << _FP_N))) >> _FP_N);
}
fp6 fp6::operator*(double i) const {
    return raw((_v * (int)round(i * (1 << _FP_N))) >> _FP_N);
}
fp6 fp6::operator*(const fp6& i) const {
    return raw((_v * i._v) >> _FP_N);
}

fp6 operator*(int i, const fp6& f) { 
    return fp6::raw(i * f._v);
}
fp6 operator*(long i, const fp6& f) { 
    return fp6::raw(i * f._v);
}
fp6 operator*(long long i, const fp6& f) { 
    return fp6::raw(i * f._v);
}
fp6 operator*(float i, const fp6& f) { 
    return fp6::raw((f._v * (int)roundf(i * (1 << _FP_N))) >> _FP_N);
}
fp6 operator*(double i, const fp6& f) { 
    return fp6::raw((f._v * (int)round(i * (1 << _FP_N))) >> _FP_N);
}

fp6& fp6::operator*=(int i) {
    _v *= i;
    return *this;
}
fp6& fp6::operator*=(long i) {
    _v *= i;
    return *this;
}
fp6& fp6::operator*=(long long i) {
    _v *= i;
    return *this;
}
fp6& fp6::operator*=(float i) {
    _v = (_v * (int)roundf(i * (1 << _FP_N))) >> _FP_N;
    return *this;
}
fp6& fp6::operator*=(double i) {
    _v = (_v * (int)round(i * (1 << _FP_N))) >> _FP_N;
    return *this;
}
fp6& fp6::operator*=(const fp6& i) {
    _v = (_v * i._v) >> _FP_N;
    return *this;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////

fp6 fp6::operator/(int i) const {
    return raw(_v / i);
}
fp6 fp6::operator/(long i) const {
    return raw(_v / i);
}
fp6 fp6::operator/(long long i) const {
    return raw(_v / i);
}
fp6 fp6::operator/(float i) const {
    return raw((_v / (int)roundf(i * (1 << _FP_N))) << _FP_N);
}
fp6 fp6::operator/(double i) const {
    return raw((_v / (int)round(i * (1 << _FP_N))) << _FP_N);
}
fp6 fp6::operator/(const fp6& i) const {
    return raw((_v / i._v) << _FP_N);
}

fp6 operator/(int i, const fp6& f) { 
    return fp6::raw((i << _FP_N) / (f._v >> _FP_N));
}
fp6 operator/(long i, const fp6& f) { 
    return fp6::raw((i << _FP_N) / (f._v >> _FP_N));
}
fp6 operator/(long long i, const fp6& f) { 
    return fp6::raw((i << _FP_N) / (f._v >> _FP_N));
}
fp6 operator/(float i, const fp6& f) { 
    return fp6::raw((((int)roundf(i * (1 << _FP_N))) / (f._v  >> _FP_N)));
}
fp6 operator/(double i, const fp6& f) { 
    return fp6::raw((((int)round(i * (1 << _FP_N))) / (f._v  >> _FP_N)));
}

fp6& fp6::operator/=(int i) {
    _v /= i;
    return *this;
}
fp6& fp6::operator/=(long i) {
    _v /= i;
    return *this;
}
fp6& fp6::operator/=(long long i) {
    _v /= i;
    return *this;
}
fp6& fp6::operator/=(float i) {
    _v = (_v / (int)roundf(i * (1 << _FP_N))) << _FP_N;
    return *this;
}
fp6& fp6::operator/=(double i) {
    _v = (_v / (int)round(i * (1 << _FP_N))) << _FP_N;
    return *this;
}
fp6& fp6::operator/=(const fp6& i) {
    _v = (_v / i._v) << _FP_N;
    return *this;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////

fp6 fp6::operator<<(int i) const {
    return raw(_v << i);
}

fp6 fp6::operator>>(int i) const {
    return raw(_v >> i);
}

fp6& fp6::operator<<=(int i) {
    _v <<= i;
    return *this;
}

fp6& fp6::operator>>=(int i) {
    _v >>= i;
    return *this;
}