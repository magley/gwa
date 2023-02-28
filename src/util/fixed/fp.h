#pragma once

#include <stdint.h>
#include <iosfwd>

#define _FP_N 6

static constexpr int FP6_MAX = (1 << (32 - _FP_N - 1)) - 1;
static constexpr int FP6_MIN = -(1 << (32 - _FP_N - 1)) + 1;

class fp6 {
public:
    
    int32_t _v;
    fp6();
    fp6(char i);
    fp6(short i);
    fp6(int i);
    fp6(long i);
    fp6(long long i);
    fp6(float i);
    fp6(double i);
    fp6(const fp6& i);
    inline static fp6 raw(int raw_val);
    fp6 abs() const;
    int sgn() const;
    fp6 clamp(const fp6& mini, const fp6& maxi) const;

    friend std::ostream& operator<<(std::ostream& other, const fp6& p);
    friend std::istream& operator>>(std::istream& other, fp6& p);

    bool operator==(char i) const;
    bool operator==(short i) const;
    bool operator==(int i) const;
    bool operator==(long i) const;
    bool operator==(long long i) const;
    bool operator==(float i) const;
    bool operator==(double i) const;
    bool operator==(const fp6& i) const;

    bool operator!=(char i) const;
    bool operator!=(short i) const;
    bool operator!=(int i) const;
    bool operator!=(long i) const;
    bool operator!=(long long i) const;
    bool operator!=(float i) const;
    bool operator!=(double i) const;
    bool operator!=(const fp6& i) const;

    bool operator<(char i) const;
    bool operator<(short i) const;
    bool operator<(int i) const;
    bool operator<(long i) const;
    bool operator<(long long i) const;
    bool operator<(float i) const;
    bool operator<(double i) const;
    bool operator<(const fp6& i) const;

    bool operator>(char i) const;
    bool operator>(short i) const;
    bool operator>(int i) const;
    bool operator>(long i) const;
    bool operator>(long long i) const;
    bool operator>(float i) const;
    bool operator>(double i) const;
    bool operator>(const fp6& i) const;

    bool operator<=(char i) const;
    bool operator<=(short i) const;
    bool operator<=(int i) const;
    bool operator<=(long i) const;
    bool operator<=(long long i) const;
    bool operator<=(float i) const;
    bool operator<=(double i) const;
    bool operator<=(const fp6& i) const;

    bool operator>=(char i) const;
    bool operator>=(short i) const;
    bool operator>=(int i) const;
    bool operator>=(long i) const;
    bool operator>=(long long i) const;
    bool operator>=(float i) const;
    bool operator>=(double i) const;
    bool operator>=(const fp6& i) const;

    fp6& operator=(char i);
    fp6& operator=(short i);
    fp6& operator=(int i);
    fp6& operator=(long i);
    fp6& operator=(long long i);
    fp6& operator=(float i);
    fp6& operator=(double i);
    fp6& operator=(const fp6& i);

    fp6 operator+(char i) const;
    fp6 operator+(short i) const;
    fp6 operator+(int i) const;
    fp6 operator+(long i) const;
    fp6 operator+(long long i) const;
    fp6 operator+(float i) const;
    fp6 operator+(double i) const;
    fp6 operator+(const fp6& i) const;

    fp6& operator+=(char i);
    fp6& operator+=(short i);
    fp6& operator+=(int i);
    fp6& operator+=(long i);
    fp6& operator+=(long long i);
    fp6& operator+=(float i);
    fp6& operator+=(double i);
    fp6& operator+=(const fp6& i);

    fp6& operator++();
    fp6 operator++(int);

    fp6 operator-(char i) const;
    fp6 operator-(short i) const;
    fp6 operator-(int i) const;
    fp6 operator-(long i) const;
    fp6 operator-(long long i) const;
    fp6 operator-(float i) const;
    fp6 operator-(double i) const;
    fp6 operator-(const fp6& i) const;

    fp6& operator-=(char i);
    fp6& operator-=(short i);
    fp6& operator-=(int i);
    fp6& operator-=(long i);
    fp6& operator-=(long long i);
    fp6& operator-=(float i);
    fp6& operator-=(double i);
    fp6& operator-=(const fp6& i);

    fp6& operator--();
    fp6 operator--(int);

    fp6 operator*(char i) const;
    fp6 operator*(short i) const;
    fp6 operator*(int i) const;
    fp6 operator*(long i) const;
    fp6 operator*(long long i) const;
    fp6 operator*(float i) const;
    fp6 operator*(double i) const;
    fp6 operator*(const fp6& i) const;

    fp6& operator*=(char i);
    fp6& operator*=(short i);
    fp6& operator*=(int i);
    fp6& operator*=(long i);
    fp6& operator*=(long long i);
    fp6& operator*=(float i);
    fp6& operator*=(double i);
    fp6& operator*=(const fp6& i);

    fp6 operator/(char i) const;
    fp6 operator/(short i) const;
    fp6 operator/(int i) const;
    fp6 operator/(long i) const;
    fp6 operator/(long long i) const;
    fp6 operator/(float i) const;
    fp6 operator/(double i) const;
    fp6 operator/(const fp6& i) const;

    fp6& operator/=(char i);
    fp6& operator/=(short i);
    fp6& operator/=(int i);
    fp6& operator/=(long i);
    fp6& operator/=(long long i);
    fp6& operator/=(float i);
    fp6& operator/=(double i);
    fp6& operator/=(const fp6& i);

    fp6 operator%(char i) const;
    fp6 operator%(short i) const;
    fp6 operator%(int i) const;
    fp6 operator%(long i) const;
    fp6 operator%(long long i) const;
    fp6 operator%(const fp6& i) const;

    fp6& operator%=(char i);
    fp6& operator%=(short i);
    fp6& operator%=(int i);
    fp6& operator%=(long i);
    fp6& operator%=(long long i);
    fp6& operator%=(const fp6& i);

    fp6 operator+() const;
    fp6 operator-() const;
    fp6 operator~() const;

    fp6 operator<<(int i) const;
    fp6 operator>>(int i) const;

    fp6& operator<<=(int i);
    fp6& operator>>=(int i);

    fp6 operator^(int i) const;
    fp6 operator&(int i) const;
    fp6 operator|(int i) const;
    fp6& operator^=(int i);
    fp6& operator&=(int i);
    fp6& operator|=(int i);

    fp6 operator^(const fp6& i) const;
    fp6 operator&(const fp6& i) const;
    fp6 operator|(const fp6& i) const;
    fp6& operator^=(const fp6& i);
    fp6& operator&=(const fp6& i);
    fp6& operator|=(const fp6& i);

    operator bool() const;
    explicit operator char() const;
    explicit operator short() const;
    explicit operator int() const;
    explicit operator long() const;
    explicit operator long long() const;
    explicit operator float() const;
    explicit operator double() const;
};

bool operator==(char i, const fp6& f);
bool operator==(short i, const fp6& f);
bool operator==(int i, const fp6& f);
bool operator==(long i, const fp6& f);
bool operator==(long long i, const fp6& f);
bool operator==(float i, const fp6& f);
bool operator==(double i, const fp6& f);

bool operator!=(char i, const fp6& f);
bool operator!=(short i, const fp6& f);
bool operator!=(int i, const fp6& f);
bool operator!=(long i, const fp6& f);
bool operator!=(long long i, const fp6& f);
bool operator!=(float i, const fp6& f);
bool operator!=(double i, const fp6& f);

bool operator<(char i, const fp6& f);
bool operator<(short i, const fp6& f);
bool operator<(int i, const fp6& f);
bool operator<(long i, const fp6& f);
bool operator<(long long i, const fp6& f);
bool operator<(float i, const fp6& f);
bool operator<(double i, const fp6& f);

bool operator>(char i, const fp6& f);
bool operator>(short i, const fp6& f);
bool operator>(int i, const fp6& f);
bool operator>(long i, const fp6& f);
bool operator>(long long i, const fp6& f);
bool operator>(float i, const fp6& f);
bool operator>(double i, const fp6& f);

bool operator<=(char i, const fp6& f);
bool operator<=(short i, const fp6& f);
bool operator<=(int i, const fp6& f);
bool operator<=(long i, const fp6& f);
bool operator<=(long long i, const fp6& f);
bool operator<=(float i, const fp6& f);
bool operator<=(double i, const fp6& f);

bool operator>=(char i, const fp6& f);
bool operator>=(short i, const fp6& f);
bool operator>=(int i, const fp6& f);
bool operator>=(long i, const fp6& f);
bool operator>=(long long i, const fp6& f);
bool operator>=(float i, const fp6& f);
bool operator>=(double i, const fp6& f);

fp6 operator+(char i, const fp6& f);
fp6 operator+(short i, const fp6& f);
fp6 operator+(int i, const fp6& f);
fp6 operator+(long i, const fp6& f);
fp6 operator+(long long i, const fp6& f);
fp6 operator+(float i, const fp6& f);
fp6 operator+(double i, const fp6& f);

fp6 operator-(char i, const fp6& f);
fp6 operator-(short i, const fp6& f);
fp6 operator-(int i, const fp6& f);
fp6 operator-(long i, const fp6& f);
fp6 operator-(long long i, const fp6& f);
fp6 operator-(float i, const fp6& f);
fp6 operator-(double i, const fp6& f);

fp6 operator*(char i, const fp6& f);
fp6 operator*(short i, const fp6& f);
fp6 operator*(int i, const fp6& f);
fp6 operator*(long i, const fp6& f);
fp6 operator*(long long i, const fp6& f);
fp6 operator*(float i, const fp6& f);
fp6 operator*(double i, const fp6& f);

fp6 operator/(char i, const fp6& f);
fp6 operator/(short i, const fp6& f);
fp6 operator/(int i, const fp6& f);
fp6 operator/(long i, const fp6& f);
fp6 operator/(long long i, const fp6& f);
fp6 operator/(float i, const fp6& f);
fp6 operator/(double i, const fp6& f);

fp6 operator%(char i, const fp6& f);
fp6 operator%(short i, const fp6& f);
fp6 operator%(int i, const fp6& f);
fp6 operator%(long i, const fp6& f);
fp6 operator%(long long i, const fp6& f);

fp6 operator&(int i, const fp6& f);
fp6 operator|(int i, const fp6& f);
fp6 operator^(int i, const fp6& f);
int& operator&=(int& i, const fp6& f);
int& operator|=(int& i, const fp6& f);
int& operator^=(int& i, const fp6& f);

fp6 min(const fp6& a, const fp6& b);
fp6 max(const fp6& a, const fp6& b);
