#include <iostream>
#include <cmath>
#include <stdexcept>

#include "util.h"
#include "Vec3.h"

Vec3::Vec3() : x(0), y(0), z(0) {}

Vec3::Vec3(float x, float y, float z) : x(x), y(y), z(z) {}

float& Vec3::operator[](int index) {
    switch (index) {
        case 0: return x;
        case 1: return y;
        case 2: return z;
        default: throw std::out_of_range("Index can only be from 0 to 3");
    }
}

float Vec3::operator[](int index) const {
    switch (index) {
        case 0: return x;
        case 1: return y;
        case 2: return z;
        default: throw std::out_of_range("Index can only be from 0 to 3");
    }
}

Vec3& Vec3::operator=(const Vec3& v) {
    x = v.x;
    y = v.y;
    z = v.z;
    return *this;
}

Vec3 operator*(const Vec3& v, const float scalar) {
    return Vec3(v.x * scalar, v.y * scalar, v.z * scalar);
}

Vec3 operator*(const float scalar, const Vec3& v) {
    return Vec3(v.x * scalar, v.y * scalar, v.z * scalar);
}

Vec3& operator*=(Vec3& v, const float scalar) {
    v.x *= scalar;
    v.y *= scalar;
    v.z *= scalar;
    return v;
}

Vec3 operator/(const Vec3& v, const float scalar) {
    return Vec3(v.x / scalar, v.y / scalar, v.z / scalar);
}

Vec3 operator/(const float scalar, const Vec3& v) {
    return Vec3(scalar / v.x, scalar / v.y, scalar / v.z);
}

Vec3& operator/=(Vec3& v, const float scalar) {
    v.x /= scalar;
    v.y /= scalar;
    v.z /= scalar;
    return v;
}

Vec3 operator+(const Vec3& v1, const Vec3& v2) {
    return Vec3(v1.x + v2.x, v1.y + v2.y, v1.z + v2.z);
}

Vec3& operator+=(Vec3& v1, const Vec3& v2) {
    v1.x += v2.x;
    v1.y += v2.y;
    v1.z += v2.z;
    return v1;
}

Vec3 operator-(const Vec3& v1, const Vec3& v2) {
    return Vec3(v1.x - v2.x, v1.y - v2.y, v1.z - v2.z);
}

Vec3& operator-=(Vec3& v1, const Vec3& v2) {
    v1.x -= v2.x;
    v1.y -= v2.y;
    v1.z -= v2.z;
    return v1;
}

std::ostream& operator<<(std::ostream& os, const Vec3& obj) {
    os << "<" << obj.x << ", " << obj.y << ", " << obj.z << ">";
    return os;
}

std::istream& operator>>(std::istream& is, Vec3& obj) {
    is >> obj.x >> obj.y >> obj.z;
    return is;
}

float Vec3::dot(const Vec3& other) const {
    return x * other.x + y * other.y + z * other.z;
}

float Vec3::norm() const {
    return sqrt(pow(x,2) + pow(y,2) + pow(z,2));
}

Vec3 Vec3::normalize() const {
    float norm = this->norm();
    if (nearlyEqual(norm, 0.0)) {
        return Vec3();
    }
    return *this / norm;
}

Vec3 Vec3::cross(Vec3& o) const {
    return Vec3(
        y*o.z - z*o.y,
        x*o.z - z*o.x,
        x*o.y - y*o.x
    );
}