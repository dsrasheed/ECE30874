#ifndef __ECE30874_VEC3_H__
#define __ECE30874_VEC3_H__

#include <iostream>

class Vec3 {
    
  public:
    float x, y, z;

    Vec3();

    Vec3(float x, float y, float z);

    float& operator[](int index);

    float operator[](int index) const;

    Vec3& operator=(const Vec3& v);

    Vec3  operator-() const;

    friend Vec3 operator*(const Vec3& v, const float scalar);

    friend Vec3 operator*(const float scalar, const Vec3& v);

    friend Vec3& operator*=(Vec3& v, const float scalar);

    friend Vec3 operator/(const Vec3& v, const float scalar);

    friend Vec3 operator/(const float scalar, const Vec3& v);

    friend Vec3& operator/=(Vec3& v, const float scalar);

    friend Vec3 operator+(const Vec3& v1, const Vec3& v2);

    friend Vec3& operator+=(Vec3& v1, const Vec3& v2);

    friend Vec3 operator-(const Vec3& v1, const Vec3& v2);

    friend Vec3& operator-=(Vec3& v1, const Vec3& v2);

    friend std::ostream& operator<<(std::ostream& os, const Vec3& obj);

    friend std::istream& operator>>(std::istream& is, Vec3& obj);

    float dot(const Vec3& other) const;

    float norm() const;

    Vec3 normalize() const;

    // BUG: Only works on declared Vec3, not ones computed in expressions
    Vec3 cross(Vec3& other) const;
};

#endif