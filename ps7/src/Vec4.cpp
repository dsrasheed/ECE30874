#include <iostream>
#include <cmath>
#include <stdexcept>

#include "util.h"
#include "Vec4.h"

Vec4::Vec4() : x(0), y(0), z(0), w(0) {}

Vec4::Vec4(float x, float y, float z, float w) : x(x), y(y), z(z), w(w) {}

float& Vec4::operator[](int index) {
    switch (index) {
        case 0: return x;
        case 1: return y;
        case 2: return z;
        case 3: return w;
        default: throw std::out_of_range("Index can only be from 0 to 3");
    }
}

float Vec4::operator[](int index) const {
    switch (index) {
        case 0: return x;
        case 1: return y;
        case 2: return z;
        default: throw std::out_of_range("Index can only be from 0 to 3");
    }
}

Vec4& Vec4::operator=(const Vec4& v) {
    x = v.x;
    y = v.y;
    z = v.z;
    w = v.w;
    return *this;
}

Vec4  Vec4::operator-() const {
    return *this * -1;
}

Vec4 operator*(const Vec4& v, const float scalar) {
    return Vec4(v.x * scalar, v.y * scalar, v.z * scalar, v.w * scalar);
}

Vec4 operator*(const float scalar, const Vec4& v) {
    return Vec4(v.x * scalar, v.y * scalar, v.z * scalar, v.w * scalar);
}

Vec4& operator*=(Vec4& v, const float scalar) {
    v.x *= scalar;
    v.y *= scalar;
    v.z *= scalar;
    v.w *= scalar;
    return v;
}

Vec4 operator/(const Vec4& v, const float scalar) {
    return Vec4(v.x / scalar, v.y / scalar, v.z / scalar, v.w / scalar);
}

Vec4 operator/(const float scalar, const Vec4& v) {
    return Vec4(scalar / v.x, scalar / v.y, scalar / v.z, scalar / v.w);
}

Vec4& operator/=(Vec4& v, const float scalar) {
    v.x /= scalar;
    v.y /= scalar;
    v.z /= scalar;
    v.w /= scalar;
    return v;
}

Vec4 operator+(const Vec4& v1, const Vec4& v2) {
    return Vec4(v1.x + v2.x, v1.y + v2.y, v1.z + v2.z, v1.w + v2.w);
}

Vec4& operator+=(Vec4& v1, const Vec4& v2) {
    v1.x += v2.x;
    v1.y += v2.y;
    v1.z += v2.z;
    v1.w += v2.w;
    return v1;
}

Vec4 operator-(const Vec4& v1, const Vec4& v2) {
    return Vec4(v1.x - v2.x, v1.y - v2.y, v1.z - v2.z, v1.w - v2.w);
}

Vec4& operator-=(Vec4& v1, const Vec4& v2) {
    v1.x -= v2.x;
    v1.y -= v2.y;
    v1.z -= v2.z;
    v1.w -= v2.w;
    return v1;
}

std::ostream& operator<<(std::ostream& os, const Vec4& obj) {
    os << "<" << obj.x << ", " << obj.y << ", " << obj.z << ", " << obj.w << ">";
    return os;
}

std::istream& operator>>(std::istream& is, Vec4& obj) {
    is >> obj.x >> obj.y >> obj.z >> obj.w;
    return is;
}

float Vec4::dot(const Vec4& other) const {
    return x * other.x + y * other.y + z * other.z + w * other.w;
}

float Vec4::norm() const {
    return sqrt(pow(x,2) + pow(y,2) + pow(z,2) + pow(w,2));
}

Vec4 Vec4::normalize() const {
    float norm = this->norm();
    if (nearlyEqual(norm, 0.0)) {
        return Vec4();
    }
    return *this / norm;
}
