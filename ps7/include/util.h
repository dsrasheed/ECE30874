#ifndef __ECE30874_UTIL_H__
#define __ECE30874_UTIL_H__

#include "Vec3.h"

extern float EPSILON;

void setEpsilon(float epsilon);

// FROM: https://floating-point-gui.de/errors/comparison/
bool nearlyEqual(float a, float b);

Vec3 windowtoNDC(const Vec3& vec);

float deg2rad(float deg);

#endif