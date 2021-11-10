#include <iostream>
#include <stdlib.h>
#include <string>
#include <cmath>

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

#include "util.h"
#include "Camera.h"
#include "Scene.h"
#include "VertexArray.h"

// Settings
#define SCR_WIDTH  700
#define SCR_HEIGHT 700
int tstep = 1;
int rstep = 10;
float fstep = 0.9;

// Scene
Shader* cam1_shader;
Camera* cam1;
Scene* scene;

// Settings
bool persp = true;
bool flatShading = true;
bool phong = true;

void exitProgram(int signal) {
  delete cam1;
  delete scene;
  cam1_shader->free();
  //fb->free();
  delete cam1_shader;
  //delete fb;
  exit(signal);
}

void processKey(unsigned char key, int x, int y) {
  switch (key) {
    case 'x':
      cam1->translate(Vec3(tstep, 0, 0));
      glutPostRedisplay();
      break;
    case 'y':
      cam1->translate(Vec3(0, tstep, 0));
      glutPostRedisplay();
      break;
    case 'z':
      cam1->translate(Vec3(0, 0, tstep));
      glutPostRedisplay();
      break;
    case 't':
      cam1->tilt(rstep);
      glutPostRedisplay();
      break;
    case 'p':
      cam1->pan(rstep);
      glutPostRedisplay();
      break;
    case 'r':
      cam1->roll(rstep);
      glutPostRedisplay();
      break;
    case 'o':
      cam1->zoom(fstep);
      glutPostRedisplay();
      break;
    case 'i':
      tstep = -tstep;
      rstep = -rstep;
      fstep = 1.0 / fstep;
      break;
    case 'w':
      cam1->translate(-cam1->get_dir());
      glutPostRedisplay();
      break;
    case 's':
      cam1->translate(cam1->get_dir());
      glutPostRedisplay();
      break;
    case 'a':
      cam1->translate(-cam1->get_right());
      glutPostRedisplay();
      break;
    case 'd':
      cam1->translate(cam1->get_right());
      glutPostRedisplay();
      break;
    case 'q':
      exitProgram(0);
  }
}

void menu(int option) {
  switch (option) {
    case 1:
      if (flatShading) {
        cam1->setShadingType(Camera::SMOOTH);
      }
      else {
        cam1->setShadingType(Camera::FLAT);
      }
      flatShading = !flatShading;
      break;
    case 2:
      if (phong) {
        cam1->setSpecType(Camera::BLINN);
      }
      else {
        cam1->setSpecType(Camera::PHONG);
      }
      phong = !phong;
      break;
    case 3:
      if (persp) {
        cam1->setProjectionType(Camera::ORTHO);
      }
      else {
        cam1->setProjectionType(Camera::PERSPECTIVE);
      }
      persp = !persp;
      break;
  }
  glutPostRedisplay();
}

void display() {
  // Render to custom framebuffer
  cam1->renderCPU(*scene);
  glDrawPixels(SCR_WIDTH, SCR_HEIGHT, GL_RGBA, GL_UNSIGNED_BYTE, cam1->getFrameBuffer().color);

  // Execute all commands
  glFlush();
  glutSwapBuffers();
}

void initGLUT(int* argc, char** argv) {
  if (*argc != 3) {
    std::cout << "Usage: ps6 scene-file camera-file" << std::endl;
    return;
  }

  glutInit(argc, argv);
  #ifdef __APPLE__
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_3_2_CORE_PROFILE);
  #else
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
  #endif
  glutInitWindowSize(SCR_WIDTH, SCR_HEIGHT);
  glutCreateWindow("ECE 30834 PS6 - drasheed");
  if (glewInit() != GLEW_OK) exit(0);

  // initialize objects
  try {
    // Cameras
    cam1_shader = new Shader("resources/shader.vs", "resources/shader.fs");
    cam1 = new Camera(argv[2], (float) SCR_WIDTH / SCR_HEIGHT, *cam1_shader);
    cam1->setFrameBuffer(SCR_WIDTH, SCR_HEIGHT);

    // Scene
    scene = new Scene();
    scene->readFromFile(argv[1]);
  }
  catch (const std::string err) {
    std::cout << err << std::endl;
    exitProgram(0);
  }

  // callbacks
  glutDisplayFunc(display);
  glutKeyboardFunc(processKey);

  // menu
  glutCreateMenu(menu);
  glutAddMenuEntry("Toggle Smooth/Flat Shading", 1);
  glutAddMenuEntry("Toggle Phong/Blinn Shading", 2);
  glutAddMenuEntry("Toggle Persp/Ortho Shading", 3);
  glutAttachMenu(GLUT_RIGHT_BUTTON);

  glutMainLoop();
}

int main(int argc, char** argv) {
  initGLUT(&argc, argv);
  return 0;
}
