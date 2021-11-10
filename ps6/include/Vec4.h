#ifndef __ECE30874_VEC4_H__
#define __ECE30874_VEC4_H__

#include <iostream>

class Vec4 {
    
  public:
    float x, y, z, w;

    Vec4();

    Vec4(float x, float y, float z, float w);

    float& operator[](int index);

    float operator[](int index) const;

    Vec4& operator=(const Vec4& v);

    Vec4  operator-() const;

    friend Vec4 operator*(const Vec4& v, const float scalar);

    friend Vec4 operator*(const float scalar, const Vec4& v);

    friend Vec4& operator*=(Vec4& v, const float scalar);

    friend Vec4 operator/(const Vec4& v, const float scalar);

    friend Vec4 operator/(const float scalar, const Vec4& v);

    friend Vec4& operator/=(Vec4& v, const float scalar);

    friend Vec4 operator+(const Vec4& v1, const Vec4& v2);

    friend Vec4& operator+=(Vec4& v1, const Vec4& v2);

    friend Vec4 operator-(const Vec4& v1, const Vec4& v2);

    friend Vec4& operator-=(Vec4& v1, const Vec4& v2);

    friend std::ostream& operator<<(std::ostream& os, const Vec4& obj);

    friend std::istream& operator>>(std::istream& is, Vec4& obj);

    float dot(const Vec4& other) const;

    float norm() const;

    Vec4 normalize() const;
};

#endif