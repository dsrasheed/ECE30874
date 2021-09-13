#ifndef __POINT_H__
#define __POINT_H__

#include <vector>
#include <math.h>

#ifdef __APPLE__
#define GL_SILENCE_DEPRECATION
#include <GLUT/glut.h>
#else
#include <glut.h>
#endif

#define PRETTY_MUCH_ZERO 1e-20
bool fuzzyEquals(float v1, float v2) {
	return abs(v1 - v2) <= PRETTY_MUCH_ZERO;
}

class LineSegment;
class Vector;
class Point;
class Ray;

class Vector {
	public:
		float x, y;

		Vector(): x(0), y(0) {}

		Vector(float x, float y): x(x), y(y) {}

		Vector normalize() {
			float u = x/float(glutGet(GLUT_WINDOW_WIDTH));
			float v = 1.0 - y/float(glutGet(GLUT_WINDOW_HEIGHT));
			return Vector(2.0f*u - 1.0f, 2.0f*v - 1.0f);
		}

		Vector operator+(Vector other) {
			return Vector(x + other.x, y + other.y);
		}

		Vector operator-(Vector other) {
			return Vector(x - other.x, y - other.y);
		}

		void operator-=(Vector other) {
			x -= other.x;
			y -= other.y;
		}

		void operator+=(Vector other) {
			x += other.x;
			y += other.y;
		}

		Vector operator*(float scalar) {
			return Vector(x * scalar, y * scalar);
		}

		Vector operator/(float scalar) {
			return Vector(x / scalar, y / scalar);
		}

		float dot(Vector other) {
			return x * other.x + y * other.y;
		}

		float norm() {
			return sqrt(pow(x, 2) + pow(y, 2));
		}

		bool isZeroVector() {
			return fuzzyEquals(x, 0) && fuzzyEquals(y, 0);
		}

		Vector normalVector() {
			float norm = this->norm();
			Vector ret = Vector(-y, x);
			if (!fuzzyEquals(norm, 0.0)) {
				ret = ret / norm;
			}
			return ret;
		}
};

class Point {
	public:
		float x, y;
		float invalid;

		Point(): x(0), y(0), invalid(true) {}

		Point(float x, float y): x(x), y(y), invalid(false) {}

		Point(Point* p): x(p->x), y(p->y), invalid(false) {}

		Point normalize() {
			float u = x/float(glutGet(GLUT_WINDOW_WIDTH));
			float v = 1.0 - y/float(glutGet(GLUT_WINDOW_HEIGHT));
			return Point(2.0f*u - 1.0f, 2.0f*v - 1.0f);
		}

		Point operator+(Vector other) {
			return Point(x + other.x, y + other.y);
		}

		Vector operator-(Point other) {
			return Vector(x - other.x, y - other.y);
		}

		static void genVerticesFromPoints(float** vertices, std::vector<Point> pts) {
			*vertices = (float *) malloc(sizeof(**vertices) * pts.size() * 2);

			for (int i = 0; i < pts.size() * 2; i += 2) {
				(*vertices)[i]   = pts[i/2].x;
				(*vertices)[i+1] = pts[i/2].y;
			}
		}
		
		static void genLineSegmentsFromPoints(std::vector<LineSegment> segs, std::vector<Point> pts);
};

class Ray {
	public:
		Point point;
		Vector vector;
		Ray(Point p, Vector v): point(p), vector(v) {}
};

class LineSegment {
	public:
		Point point;
		Vector vector;

		LineSegment(): point(Point()), vector(Vector()) {}
		
		LineSegment(Point p1, Point p2) {
			point = p1;
			vector = p2 - p1;
		}

		LineSegment(Point p, Vector v): point(p), vector(v) {}

		static void genLineSegmentsFromPoints(std::vector<LineSegment>* segs, std::vector<Point> pts)  {
			segs->clear();
			for (int i = 0; i < pts.size() - 1; i++) {
				segs->push_back(LineSegment(pts[i], pts[i+1]));
			}
			segs->push_back(LineSegment(pts[pts.size() - 1], pts[0]));
		}
};

#endif
