#ifndef __POLYGON_H__
#define __POLYGON_H__

#ifdef __APPLE__
#define GL_SILENCE_DEPRECATION
#include <GL/glew.h>
#include <OpenGL/gl.h>
#include <GLUT/glut.h>
#else
#include <GL/glew.h>
#include <GL/gl.h>
#include <GL/glut.h>
#endif

#include <stdlib.h>
#include <math.h>
#include "Point.h"
#include "VertexArray2.h"
#include "Shader.h"

#define POLY_VERT_SHADER_PATH "poly.vs"
#define POLY_FRAG_SHADER_PATH "poly.fs"

enum PolygonDrawMode {
   OPEN, CLOSED
};

class Polygon {
	private:

    int    n_vertices;
    float* vertices;
    PolygonDrawMode dm;
    VertexArray va;
    Shader sh;

	public:
        bool invalid;

        Polygon() {}

        Polygon(int n_vertices, float* vertices) {
            this->vertices = vertices;
            this->n_vertices = n_vertices;
            this->dm = CLOSED;
            this->va = VertexArray(n_vertices, vertices);
            this->sh = Shader(POLY_VERT_SHADER_PATH, POLY_FRAG_SHADER_PATH);
            this->invalid = false;
        }

        void setDrawMode(PolygonDrawMode dm) {
            this->dm = dm;
        }

        void draw() {
            this->va.use();
            this->sh.use();
            glDrawArrays(this->dm == CLOSED ? GL_LINE_LOOP : GL_LINE_STRIP, 0, this->n_vertices);
            this->sh.unuse();
        }

        void free() {
            this->va.free();
            this->sh.free();
            std::free(this->vertices);
            this->invalid = true;
        }
};

#endif
