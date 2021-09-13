#include <iostream>
#include <assert.h>
#include <math.h>

#include "Point.h"
#include "collisions.h"

void printPoint(Point p) {
    std::cout << "(" << p.x << ", " << p.y << ")" << std::endl;
}

int main() {
    // Intersection
    Ray r = Ray(Point(0, 2), Vector(2, -2));
    LineSegment l = LineSegment(Point(0, 0), Vector(1, 1));
    Point p = intersection(r, l);
    assert(abs(p.x - 1) <= PRETTY_MUCH_ZERO);
    assert(abs(p.y - 1) <= PRETTY_MUCH_ZERO);

    // No intersection
    r = Ray(Point(0, 0), Vector(1, 1));
    l = LineSegment(Point(-2,-2), Vector(-2, -2));
    p = intersection(r, l);
    assert(abs(p.x) <= PRETTY_MUCH_ZERO);
    assert(abs(p.y) <= PRETTY_MUCH_ZERO);
    assert(p.invalid);

    // Decimal Intersection
    r = Ray(Point(0.7, 0.6), Vector(0.6, -0.7));
    l = LineSegment(Point(1.0, 1.0), Vector(-1, -1));
    p = intersection(r, l);
    assert(!p.invalid);
    printPoint(p);

    // Parallel Lines
    r = Ray(Point(0.0, 0.0), Vector(0.0, 1.0));
    l = LineSegment(Point(1.0, 0.0), Vector(0.0, 1.0));
    p = intersection(r, l);
    assert(p.invalid);

    // Slight tilt
    r = Ray(Point(-1.0, 0.0), Vector(0.001, 1));
    l = LineSegment(Point(1.0, 0.0), Vector(-1, 2000));
    p = intersection(r, l);
    assert(!p.invalid);
    printPoint(p);

    // Lines Intersect but not segments
    r = Ray(Point(-1, -1), Vector(1, 1));
    l = LineSegment(Point(-1, 1), Vector(0.5, -0.5));
    p = intersection(r, l);
    assert(p.invalid);
    l.vector = Vector(1,-1);
    p = intersection(r, l);
    assert(!p.invalid);
    printPoint(p);
}