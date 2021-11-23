#ifndef VERTEXARRAY
#define VERTEXARRAY

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

class VertexArray {
  protected:
  unsigned int vbo, vao;

  public:
  VertexArray () : vbo(0), vao(0) {}

  void free () {
    if (vbo != 0u)
      glDeleteBuffers(1, &vbo);
    if (vao != 0u)
      glDeleteVertexArrays(1, &vao);
  }
  
  void use () {
    glBindVertexArray(vao);
  }
};

class VertexArray6 : public VertexArray {
public:
  VertexArray6 (unsigned int nv, float *vs) {
    unsigned long sf = sizeof(float), stride = 6u*sf;
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, nv*stride, vs, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, (void*) 0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, stride, (void*) (3u*sf));
    glEnableVertexAttribArray(1);
  }
};

class VertexArray8: public VertexArray {
public:
  VertexArray8 (unsigned int nv, float *vs) {
    unsigned long sf = sizeof(float), stride = 8u*sf;
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, nv*stride, vs, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, (void*) 0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, stride, (void*) (3u*sf));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, stride, (void*) (6u*sf));
    glEnableVertexAttribArray(2);
  }
};

#endif
