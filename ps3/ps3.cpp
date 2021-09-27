#include <iostream>
#include <stdlib.h>

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

#include "Camera.h"
#include "Scene.h"

// Settings
#define SCR_WIDTH  800
#define SCR_HEIGHT 600

int tstep = 1;
int rstep = 10;
float fstep = 0.9;

Shader* s;
Camera* cam;
Scene* scene;

void exitProgram(int signal) {
  delete cam;
  delete scene;
  s->free();
  delete s;
  exit(signal);
}

void processKey(unsigned char key, int x, int y) {
  switch (key) {
    case 'x':
      cam->translate(Vec3(tstep, 0, 0));
      glutPostRedisplay();
      break;
    case 'y':
      cam->translate(Vec3(0, tstep, 0));
      glutPostRedisplay();
      break;
    case 'z':
      cam->translate(Vec3(0, 0, tstep));
      glutPostRedisplay();
      break;
    case 't':
      cam->tilt(rstep);
      glutPostRedisplay();
      break;
    case 'p':
      cam->pan(rstep);
      glutPostRedisplay();
      break;
    case 'r':
      cam->roll(rstep);
      glutPostRedisplay();
      break;
    case 'o':
      cam->zoom(fstep);
      glutPostRedisplay();
      break;
    case 'i':
      tstep = -tstep;
      rstep = -rstep;
      fstep = 1.0 / fstep;
      break;
    case 'q':
      exitProgram(0);
  }
}

void display() {
  glClearColor(1.0, 1.0, 1.0, 1.0);
  glEnable(GL_DEPTH_TEST);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  cam->render(*scene);
  glFlush();
  glutSwapBuffers();
}

void initGLUT(int* argc, char** argv) {
  glutInit(argc, argv);
  #ifdef __APPLE__
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH | GLUT_3_2_CORE_PROFILE);
  #else
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
  #endif
  glutInitWindowSize(SCR_WIDTH, SCR_HEIGHT);
  glutCreateWindow("ECE 30874 Problem Set 3");
  if (glewInit() != GLEW_OK) exit(0);

  // initialize objects
  s = new Shader("resources/shader.vs", "resources/shader.fs");
  cam = new Camera(45, (float) SCR_WIDTH / SCR_HEIGHT, 1, 100, *s);
  scene = new Scene();
  scene->readFromFile("resources/scene1-3");

  // callbacks
  glutDisplayFunc(display);
  //glutKeyboardUpFunc(processKey);
  glutKeyboardFunc(processKey);

  glutMainLoop();
}

int main(int argc, char** argv) {
  initGLUT(&argc, argv);
  return 0;
}
