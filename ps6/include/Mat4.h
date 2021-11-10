#ifndef __ECE30874_Mat4_H__
#define __ECE30874_Mat4_H__

#include <iostream>
#include "Vec4.h"

class Mat4 {
  private:
    Vec4 row1, row2, row3, row4;

  public:

    Mat4();

    Mat4(float data[]);

    Mat4(float v1, float v2, float v3, float v4, 
         float v5, float v6, float v7, float v8, 
         float v9, float v10, float v11, float v12,
         float v13, float v14, float v15, float v16);

    Mat4(Vec4 r1, Vec4 r2, Vec4 r3, Vec4 r4);

    float* data();

    Vec4& operator[](int index);

    const Vec4& operator[](int index) const;

    Mat4& operator=(const Mat4& m);

    friend Mat4 operator+(const Mat4& m1, const Mat4& m2);

    friend Mat4& operator+=(Mat4& m1, const Mat4& m2);

    friend Mat4 operator-(const Mat4& m1, const Mat4& m2);

    friend Mat4& operator-=(Mat4& m1, const Mat4& m2);

    friend Mat4 operator*(const Mat4& m, const float scalar);

    friend Mat4 operator*(const float scalar, Mat4& m);

    friend Mat4& operator*=(Mat4& m, const float scalar);

    friend Vec4 operator*(const Mat4& m, const Vec4& v);

    friend Mat4 operator*(const Mat4& m1, const Mat4& m2);

    friend std::ostream& operator<<(std::ostream& os, const Mat4& obj);

    friend std::istream& operator>>(std::istream& is, Mat4& obj);
};

#endif