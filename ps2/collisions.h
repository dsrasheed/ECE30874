#ifndef __COLLISSIONS_H__
#define __COLLISSIONS_H__

#include "Point.h"
#include <math.h>

Point intersection(Ray ray, LineSegment segment) {
    Point  R = ray.point;
    Vector u = ray.vector;
    Point  L = segment.point;
    Vector v = segment.vector;

    // beta = scalar to multiply the ray's vector by
    float alpha_denom = u.y * v.x - u.x * v.y;
    if (abs(alpha_denom) <= PRETTY_MUCH_ZERO) {
        return Point();
    }

    float alpha = (R.x * v.y - R.y * v.x - L.x * v.y + L.y * v.x) / alpha_denom;
    if (alpha < 0) {
        return Point();
    }

    // Line segment vector scalar
    float beta;
    if (abs(v.x) > PRETTY_MUCH_ZERO) {
        beta = (R.x + alpha * u.x - L.x) / v.x;
    }
    else if (abs(v.y) > PRETTY_MUCH_ZERO) {
        beta = (R.y + alpha * u.y - L.y) / v.y;
    }
    else {
        return Point();
    }

    // If beta is greater than 1 or less than 0, that means the ray intersects 
    // with the line, but not the line segment.
    if (beta < 0 || 1.0 < beta) {
        return Point();
    }
    return Point(R + u * alpha);
}


#endif