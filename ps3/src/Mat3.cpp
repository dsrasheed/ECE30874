#include <stdexcept>
#include <iostream>

#include "Vec3.h"
#include "Mat3.h"


Mat3::Mat3() : row1(Vec3()), row2(Vec3()), row3(Vec3()) {}

Mat3::Mat3(float data[]) {
    row1 = Vec3(data[0], data[1], data[2]);
    row2 = Vec3(data[3], data[4], data[5]);
    row3 = Vec3(data[6], data[7], data[8]);
}

Mat3::Mat3(float v1, float v2, float v3, float v4, float v5, float v6, float v7, float v8, float v9) {
    row1 = Vec3(v1, v2, v3);
    row2 = Vec3(v4, v5, v6);
    row3 = Vec3(v7, v8, v9);
}

Mat3::Mat3(Vec3 r1, Vec3 r2, Vec3 r3): row1(r1), row2(r2), row3(r3) {}

float* Mat3::data() {
    float* dat = new float[9];
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            dat[i*3+j] = (*this)[i][j];
        }
    }
    return dat;
}

Vec3& Mat3::operator[](int index) {
    switch (index) {
        case 0: return row1;
        case 1: return row2;
        case 2: return row3;
        default: throw std::out_of_range("Index can only be from 0 to 3");
    }
}

const Vec3& Mat3::operator[](int index) const {
    switch (index) {
        case 0: return row1;
        case 1: return row2;
        case 2: return row3;
        default: throw std::out_of_range("Index can only be from 0 to 3");
    }
}

Mat3& Mat3::operator=(const Mat3& m) {
    row1.x = m.row1.x;
    row1.y = m.row1.y;
    row1.z = m.row1.z;
    row2.x = m.row2.x;
    row2.y = m.row2.y;
    row2.z = m.row2.z;
    row3.x = m.row3.x;
    row3.y = m.row3.y;
    row3.z = m.row3.z;
    return *this;
}

Mat3 operator+(const Mat3& m1, const Mat3& m2) {
    return Mat3(m1.row1.x + m2.row1.x, m1.row1.y + m2.row1.y, m1.row1.z + m2.row1.z,
        m1.row2.x + m2.row2.x, m1.row2.y + m2.row2.y, m1.row2.z + m2.row2.z,
        m1.row3.x + m2.row3.x, m1.row3.y + m2.row3.y, m1.row3.z + m2.row3.z
    );
}

Mat3& operator+=(Mat3& m1, const Mat3& m2) {
    m1.row1.x += m2.row1.x;
    m1.row1.y += m2.row1.y;
    m1.row1.z += m2.row1.z;
    m1.row2.x += m2.row2.x;
    m1.row2.y += m2.row2.y;
    m1.row2.z += m2.row2.z;
    m1.row3.x += m2.row3.x;
    m1.row3.y += m2.row3.y;
    m1.row3.z += m2.row3.z;
    return m1;
}

Mat3 operator-(const Mat3& m1, const Mat3& m2) {
    return Mat3(m1.row1.x - m2.row1.x, m1.row1.y - m2.row1.y, m1.row1.z - m2.row1.z,
        m1.row2.x - m2.row2.x, m1.row2.y - m2.row2.y, m1.row2.z - m2.row2.z,
        m1.row3.x - m2.row3.x, m1.row3.y - m2.row3.y, m1.row3.z - m2.row3.z
    );
}

Mat3& operator-=(Mat3& m1, const Mat3& m2) {
    m1.row1.x -= m2.row1.x;
    m1.row1.y -= m2.row1.y;
    m1.row1.z -= m2.row1.z;
    m1.row2.x -= m2.row2.x;
    m1.row2.y -= m2.row2.y;
    m1.row2.z -= m2.row2.z;
    m1.row3.x -= m2.row3.x;
    m1.row3.y -= m2.row3.y;
    m1.row3.z -= m2.row3.z;
    return m1;
}

Mat3 operator*(const Mat3& m, const float scalar) {
    return Mat3(
        m.row1.x * scalar,
        m.row1.y * scalar,
        m.row1.z * scalar,
        m.row2.x * scalar,
        m.row2.y * scalar,
        m.row2.z * scalar,
        m.row3.x * scalar,
        m.row3.y * scalar,
        m.row3.z * scalar
    );
}

Mat3 operator*(const float scalar, Mat3& m) {
    return Mat3(
        m.row1.x * scalar,
        m.row1.y * scalar,
        m.row1.z * scalar,
        m.row2.x * scalar,
        m.row2.y * scalar,
        m.row2.z * scalar,
        m.row3.x * scalar,
        m.row3.y * scalar,
        m.row3.z * scalar
    );
}

Mat3& operator*=(Mat3& m, const float scalar) {
    m.row1.x *= scalar;
    m.row1.y *= scalar;
    m.row1.z *= scalar;
    m.row2.x *= scalar;
    m.row2.y *= scalar;
    m.row2.z *= scalar;
    m.row3.x *= scalar;
    m.row3.y *= scalar;
    m.row3.z *= scalar;
    return m;
}

Vec3 operator*(const Mat3& m, const Vec3& v) {
    return Vec3(
        m.row1.dot(v),
        m.row2.dot(v),
        m.row3.dot(v)
    );
}

Mat3 operator*(const Mat3& m1, const Mat3& m2) {
    Vec3 col1 = Vec3(m2.row1.x, m2.row2.x, m2.row3.x);
    Vec3 col2 = Vec3(m2.row1.y, m2.row2.y, m2.row3.y);
    Vec3 col3 = Vec3(m2.row1.z, m2.row2.z, m2.row3.z);
    return Mat3(
        m1.row1.dot(col1),
        m1.row1.dot(col2),
        m1.row1.dot(col3),
        m1.row2.dot(col1),
        m1.row2.dot(col2),
        m1.row2.dot(col3),
        m1.row3.dot(col1),
        m1.row3.dot(col2),
        m1.row3.dot(col3)
    );
}

std::ostream& operator<<(std::ostream& os, const Mat3& obj) {
    os << "["  << obj.row1.x << ", " << obj.row1.y << ", " << obj.row1.z << std::endl
                << obj.row2.x << ", " << obj.row2.y << ", " << obj.row2.z << std::endl
                << obj.row3.x << ", " << obj.row3.y << ", " << obj.row3.z << "]";
    return os;
}

std::istream& operator>>(std::istream& is, Mat3& obj) {
    is >> obj.row1.x >> obj.row1.y >> obj.row1.z
       >> obj.row2.x >> obj.row2.y >> obj.row2.z
       >> obj.row3.x >> obj.row3.y >> obj.row3.z;
    return is;
}