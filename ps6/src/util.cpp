#include <cmath>
#include <cfloat>

#ifdef __APPLE__
#define GL_SILENCE_DEPRECATION
#include <GL/glew.h>
#include <OpenGL/gl.h>
#include <GLUT/glut.h>
#else
// TODO: Ask what headers should be for them
#include <GL/glew.h>
#include <GL/gl.h>
#include <GL/glut.h>
#endif

#include "Vec3.h"

float EPSILON = 0.000001;

void setEpsilon(float epsilon) {
    EPSILON = epsilon;
}

// FROM: https://floating-point-gui.de/errors/comparison/
bool nearlyEqual(float a, float b) {
		const float absA = abs(a);
		const float absB = abs(b);
		const float diff = abs(a - b);

		if (a == b) { // shortcut, handles infinities
			return true;
        }
	    return diff / fmin((absA + absB), FLT_MAX) < EPSILON;
}

Vec3 windowtoNDC(const Vec3& vec) {
	float u = vec[0]/float(glutGet(GLUT_WINDOW_WIDTH));
	float v = 1.0 - vec[1]/float(glutGet(GLUT_WINDOW_HEIGHT));
	return Vec3(2.0f*u - 1.0f, 2.0f*v - 1.0f, vec[2]);
}

float deg2rad(float deg) {
	return deg * M_PI / 180.0;
}
