#ifndef __BALL_H__
#define __BALL_H__

#ifdef __APPLE__
#define GL_SILENCE_DEPRECATION
#include <GL/glew.h>
#include <OpenGL/gl.h>
#include <GLUT/glut.h>
#else
// TODO: Ask what headers should be for them
#include <GL/glut.h>
#endif

#include <stdlib.h>
#include <math.h>
#include "Point.h"
#include "VertexArray2.h"
#include "Shader.h"

#define BALL_VERT_SHADER_PATH "ball.vs"
#define BALL_FRAG_SHADER_PATH "ball.fs"

class Ball {
	private:

    int   n_vertices;
    float* vertices;
    Point center;
    float radius;
    VertexArray va;
    Shader sh;
    
		unsigned int calcNVertices(float errorPercent) {
			float theta = acos(1.0 - errorPercent);
			return (unsigned int) ceil((2 * M_PI) / theta);
		}

		void genVertices(int* n_vertices, float** vertices, float errorPercent) {
			*n_vertices = this->calcNVertices(errorPercent);
      *n_vertices += 2; // for center and repeat point
      *vertices = (float *) malloc(sizeof(*vertices) * (*n_vertices) * 2);

      // Center
      (*vertices)[0] = 0.0;
      (*vertices)[1] = 0.0;

      // Perimeter
      float delta_theta = (2 * M_PI) / (*n_vertices - 1);
      float theta = 0.0;
      for (int i = 2; i < (*n_vertices - 1) * 2; i += 2) {
        (*vertices)[i]   = cos(theta);
        (*vertices)[i+1] = sin(theta);
        theta += delta_theta;
      }

      // Repeat first perimeter vertex
      (*vertices)[(*n_vertices - 1) * 2 + 0] = (*vertices)[2];
      (*vertices)[(*n_vertices - 1) * 2 + 1] = (*vertices)[3];
		}

    void init(float errorPercent) {
      // Create ball vertices
      this->genVertices(&(this->n_vertices), &(this->vertices), errorPercent);

      // Create vertex array
      this->va = VertexArray(this->n_vertices, this->vertices);
      
      // Create shader
      this->sh = Shader(BALL_VERT_SHADER_PATH, BALL_FRAG_SHADER_PATH);

      this->setCenter(Point(0, 0));
      this->setRadius(1.0);
    }

	public:

    Ball() {}

    Ball(float errorPercent) {
      this->init(errorPercent);
    }

    // expects normalized point
    void setRadiusUsingPointOnPerimeter(Point onPerimeter) {
      float a_sq = pow(this->center.x - onPerimeter.x, 2);
      float b_sq = pow(this->center.y - onPerimeter.y, 2);
      this->setRadius(sqrt(a_sq + b_sq));
    }
    
    // expects normalized radius
    void setRadius(float radius) {
      this->radius = radius;
      float scale[] = {
        radius, 0.0,    0.0,    0.0,
        0.0,    radius, 0.0,    0.0,
        0.0,    0.0,    radius, 0.0,
        0.0,    0.0,    0.0,    1.0
      };
      this->sh.use();
      this->sh.setMat4("scale", scale);
      this->sh.unuse();
    }

    // expects normalized point
    void setCenter(Point center) {
      this->center = center;
      float translate[] = {
        1.0, 0.0, 0.0, center.x,
        0.0, 1.0, 0.0, center.y,
        0.0, 0.0, 1.0, 0.0,
        0.0, 0.0, 0.0, 1.0
      };
      this->sh.use();
      this->sh.setMat4("translate", translate);
      this->sh.unuse();
    }

    Point getCenter() { return this->center; }
    float getRadius() { return this->radius; }

		void draw() {
			this->va.use();
			this->sh.use();
			glDrawArrays(GL_TRIANGLE_FAN, 0, this->n_vertices);
      this->sh.unuse();
		}

    void free() {
      this->va.free();
      this->sh.free();
      std::free(this->vertices);
    }
};

#endif
