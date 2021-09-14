#ifndef VERTEXARRAY
#define VERTEXARRAY

#ifdef __APPLE__
#define GL_SILENCE_DEPRECATION
#include <GL/glew.h>
#include <OpenGL/gl.h>
#include <GLUT/glut.h>
#else
#include <GL/gl.h>
#include <GL/glut.h>
#endif

class VertexArray {
  unsigned int vbo, vao;
public:
  VertexArray () : vao(0), vbo(0) {}

  VertexArray (unsigned int nv, float *vs) {
    unsigned long stride = 2u*sizeof(float);
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, nv*stride, vs, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 2u, GL_FLOAT, GL_FALSE, stride, (void*) 0);
    glEnableVertexAttribArray(0);
  }

  void free () {
    glDeleteBuffers(1, &vbo);
    glDeleteVertexArrays(1, &vao);
  }
  
  void use () {
    glBindVertexArray(vao);
  }
};

class VertexArrayIndex : public VertexArray {
  unsigned int ebo;
public:
  VertexArrayIndex (unsigned int nv, float *vs, unsigned int ni, unsigned int *is)
    : VertexArray(nv, vs) {
    glGenBuffers(1, &ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, ni*sizeof(unsigned int), is, GL_STATIC_DRAW); 
  }

  void free () {
    glDeleteBuffers(1, &ebo);
    VertexArray::free();
  }
};

#endif
