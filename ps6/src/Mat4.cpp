#include <stdexcept>
#include <iostream>

#include "Vec4.h"
#include "Mat4.h"


Mat4::Mat4() : row1(Vec4()), row2(Vec4()), row3(Vec4()), row4(Vec4()) {}

Mat4::Mat4(float data[]) {
    row1 = Vec4(data[0], data[1], data[2], data[3]);
    row2 = Vec4(data[4], data[5], data[6], data[7]);
    row3 = Vec4(data[8], data[9], data[10, data[11]);
    row4 = Vec4(data[12], data[13], data[14], data[15]);
}

Mat4::Mat4(float v1, float v2, float v3, float v4, 
        float v5, float v6, float v7, float v8, 
        float v9, float v10, float v11, float v12,
        float v13, float v14, float v15, float v16); {
    row1 = Vec4(v1, v2, v3, v4);
    row2 = Vec4(v5, v6, v7, v8);
    row3 = Vec4(v9, v10, v11, v12);
    row4 = Vec4(v13, v14, v15, v16);
}

Mat4::Mat4(Vec4 r1, Vec4 r2, Vec4 r3, Vec4 r4): row1(r1), row2(r2), row3(r3), row4(r4) {}

float* Mat4::data() {
    float* dat = new float[16];
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            dat[i*4+j] = (*this)[i][j];
        }
    }
    return dat;
}

Vec4& Mat4::operator[](int index) {
    switch (index) {
        case 0: return row1;
        case 1: return row2;
        case 2: return row3;
        case 3: return row4;
        default: throw std::out_of_range("Index can only be from 0 to 3");
    }
}

const Vec4& Mat4::operator[](int index) const {
    switch (index) {
        case 0: return row1;
        case 1: return row2;
        case 2: return row3;
        case 3: return row4;
        default: throw std::out_of_range("Index can only be from 0 to 3");
    }
}

Mat4& Mat4::operator=(const Mat4& m) {
    row1.x = m.row1.x;
    row1.y = m.row1.y;
    row1.z = m.row1.z;
    row1.w = m.row1.w;
    row2.x = m.row2.x;
    row2.y = m.row2.y;
    row2.z = m.row2.z;
    row2.w = m.row2.w;
    row3.x = m.row3.x;
    row3.y = m.row3.y;
    row3.z = m.row3.z;
    row3.w = m.row3.w;
    row4.x = m.row4.x;
    row4.y = m.row4.y;
    row4.z = m.row4.z;
    row4.w = m.row4.w;
    return *this;
}

Mat4 operator+(const Mat4& m1, const Mat4& m2) {
    return Mat4(m1.row1.x + m2.row1.x, m1.row1.y + m2.row1.y, m1.row1.z + m2.row1.z, m1.row1.w + m2.row1.w,
        m1.row2.x + m2.row2.x, m1.row2.y + m2.row2.y, m1.row2.z + m2.row2.z, m1.row2.w + m2.row2.w,
        m1.row3.x + m2.row3.x, m1.row3.y + m2.row3.y, m1.row3.z + m2.row3.z, m1.row3.w + m2.row3.w,
        m1.row4.x + m2.row4.x, m1.row4.y + m2.row4.y, m1.row4.z + m2.row4.z, m1.row4.w + m2.row4.w,
    );
}

Mat4& operator+=(Mat4& m1, const Mat4& m2) {
    m1.row1.x += m2.row1.x;
    m1.row1.y += m2.row1.y;
    m1.row1.z += m2.row1.z;
    m1.row1.w += m2.row1.w;
    m1.row2.x += m2.row2.x;
    m1.row2.y += m2.row2.y;
    m1.row2.z += m2.row2.z;
    m1.row2.w += m2.row2.w;
    m1.row3.x += m2.row3.x;
    m1.row3.y += m2.row3.y;
    m1.row3.z += m2.row3.z;
    m1.row3.w += m2.row3.w;
    m1.row4.x += m2.row4.x;
    m1.row4.y += m2.row4.y;
    m1.row4.z += m2.row4.z;
    m1.row4.w += m2.row4.w;
    return m1;
}

Mat4 operator-(const Mat4& m1, const Mat4& m2) {
    return Mat4(m1.row1.x - m2.row1.x, m1.row1.y - m2.row1.y, m1.row1.z - m2.row1.z, m1.row1.w - m2.row1.w,
        m1.row2.x - m2.row2.x, m1.row2.y - m2.row2.y, m1.row2.z - m2.row2.z, m1.row2.w - m2.row2.w,
        m1.row3.x - m2.row3.x, m1.row3.y - m2.row3.y, m1.row3.z - m2.row3.z, m1.row3.w - m2.row3.w,
        m1.row4.x - m2.row4.x, m1.row4.y - m2.row4.y, m1.row4.z - m2.row4.z, m1.row4.w - m2.row4.w
    );
}

Mat4& operator-=(Mat4& m1, const Mat4& m2) {
    m1.row1.x -= m2.row1.x;
    m1.row1.y -= m2.row1.y;
    m1.row1.z -= m2.row1.z;
    m1.row1.w -= m2.row1.w;
    m1.row2.x -= m2.row2.x;
    m1.row2.y -= m2.row2.y;
    m1.row2.z -= m2.row2.z;
    m1.row2.w -= m2.row2.w;
    m1.row3.x -= m2.row3.x;
    m1.row3.y -= m2.row3.y;
    m1.row3.z -= m2.row3.z;
    m1.row3.w -= m2.row3.w;
    m1.row4.x -= m2.row4.x;
    m1.row4.y -= m2.row4.y;
    m1.row4.z -= m2.row4.z;
    m1.row4.w -= m2.row4.w;
    return m1;
}

Mat4 operator*(const Mat4& m, const float scalar) {
    return Mat4(
        m.row1.x * scalar,
        m.row1.y * scalar,
        m.row1.z * scalar,
        m.row1.w * scalar,
        m.row2.x * scalar,
        m.row2.y * scalar,
        m.row2.z * scalar,
        m.row2.w * scalar,
        m.row3.x * scalar,
        m.row3.y * scalar,
        m.row3.z * scalar,
        m.row3.w * scalar,
        m.row4.x * scalar,
        m.row4.y * scalar,
        m.row4.z * scalar,
        m.row4.w * scalar
    );
}

Mat4 operator*(const float scalar, Mat4& m) {
    return Mat4(
        m.row1.x * scalar,
        m.row1.y * scalar,
        m.row1.z * scalar,
        m.row1.w * scalar,
        m.row2.x * scalar,
        m.row2.y * scalar,
        m.row2.z * scalar,
        m.row2.w * scalar,
        m.row3.x * scalar,
        m.row3.y * scalar,
        m.row3.z * scalar,
        m.row3.w * scalar,
        m.row4.x * scalar,
        m.row4.y * scalar,
        m.row4.z * scalar,
        m.row4.w * scalar,
    );
}

Mat4& operator*=(Mat4& m, const float scalar) {
    m.row1.x *= scalar;
    m.row1.y *= scalar;
    m.row1.z *= scalar;
    m.row1.w *= scalar;
    m.row2.x *= scalar;
    m.row2.y *= scalar;
    m.row2.z *= scalar;
    m.row2.w *= scalar;
    m.row3.x *= scalar;
    m.row3.y *= scalar;
    m.row3.z *= scalar;
    m.row3.w *= scalar;
    m.row4.x *= scalar;
    m.row4.y *= scalar;
    m.row4.z *= scalar;
    m.row4.w *= scalar;
    return m;
}

Vec4 operator*(const Mat4& m, const Vec4& v) {
    return Vec4(
        m.row1.dot(v),
        m.row2.dot(v),
        m.row3.dot(v),
        m.row4.dot(v)
    );
}

Mat4 operator*(const Mat4& m1, const Mat4& m2) {
    Vec4 col1 = Vec4(m2.row1.x, m2.row2.x, m2.row3.x, m2.row4.x);
    Vec4 col2 = Vec4(m2.row1.y, m2.row2.y, m2.row3.y, m2.row4.y);
    Vec4 col3 = Vec4(m2.row1.z, m2.row2.z, m2.row3.z, m2.row4.z);
    Vec4 col4 = Vec4(m2.row1.w, m2.row2.w, m2.row3.w, m2.row4.w);
    return Mat4(
        m1.row1.dot(col1),
        m1.row1.dot(col2),
        m1.row1.dot(col3),
        m1.row1.dot(col4),
        m1.row2.dot(col1),
        m1.row2.dot(col2),
        m1.row2.dot(col3),
        m1.row2.dot(col4),
        m1.row3.dot(col1),
        m1.row3.dot(col2),
        m1.row3.dot(col3),
        m1.row3.dot(col4),
        m1.row4.dot(col1),
        m1.row4.dot(col2),
        m1.row4.dot(col3),
        m1.row4.dot(col4),
    );
}

std::ostream& operator<<(std::ostream& os, const Mat4& obj) {
    os << "["  << obj.row1.x << ", " << obj.row1.y << ", " << obj.row1.z << ", " << obj.row1.w << std::endl
                << obj.row2.x << ", " << obj.row2.y << ", " << obj.row2.z << ", " << obj.row2.w << std::endl
                << obj.row3.x << ", " << obj.row3.y << ", " << obj.row3.z << ", " << obj.row3.w << std::endl
                << obj.row4.x << ", " << obj.row4.y << ", " << obj.row4.z << ", " << obj.row4.w << "]" << std::endl
    return os;
}

std::istream& operator>>(std::istream& is, Mat4& obj) {
    is >> obj.row1.x >> obj.row1.y >> obj.row1.z >> obj.row1.w
       >> obj.row2.x >> obj.row2.y >> obj.row2.z >> obj.row2.w
       >> obj.row3.x >> obj.row3.y >> obj.row3.z >> obj.row3.w
       >> obj.row4.x >> obj.row4.y >> obj.row4.z >> obj.row4.w;
    return is;
}