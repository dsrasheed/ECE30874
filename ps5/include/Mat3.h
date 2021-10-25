#ifndef __ECE30874_MAT3_H__
#define __ECE30874_MAT3_H__

#include <iostream>
#include "Vec3.h"

class Mat3 {
  private:
    Vec3 row1, row2, row3;

  public:

    Mat3();

    Mat3(float data[]);

    Mat3(float v1, float v2, float v3, float v4, float v5, float v6, float v7, float v8, float v9);

    Mat3(Vec3 r1, Vec3 r2, Vec3 r3);

    float* data();

    Vec3& operator[](int index);

    const Vec3& operator[](int index) const;

    Mat3& operator=(const Mat3& m);

    friend Mat3 operator+(const Mat3& m1, const Mat3& m2);

    friend Mat3& operator+=(Mat3& m1, const Mat3& m2);

    friend Mat3 operator-(const Mat3& m1, const Mat3& m2);

    friend Mat3& operator-=(Mat3& m1, const Mat3& m2);

    friend Mat3 operator*(const Mat3& m, const float scalar);

    friend Mat3 operator*(const float scalar, Mat3& m);

    friend Mat3& operator*=(Mat3& m, const float scalar);

    friend Vec3 operator*(const Mat3& m, const Vec3& v);

    friend Mat3 operator*(const Mat3& m1, const Mat3& m2);

    friend std::ostream& operator<<(std::ostream& os, const Mat3& obj);

    friend std::istream& operator>>(std::istream& is, Mat3& obj);
};

#endif