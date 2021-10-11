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
#define PANE_WIDTH 700
#define PANE_HEIGHT 700
#define SCR_WIDTH  (PANE_WIDTH+20+PANE_WIDTH)
#define SCR_HEIGHT PANE_HEIGHT
int tstep = 1;
int rstep = 10;
float fstep = 0.9;

// Divider
const float divider_vs[] = {
  -1.0, 1.0, -1.0,
  -1.0, -1.0, -1.0,
  1.0, -1.0, -1.0,
  1.0, 1.0, -1.0
};
const unsigned int divider_is[] = {
  0, 1, 2,
  0, 2, 3
};
const int nDividerVerts = sizeof(divider_vs) / sizeof(typeof(*divider_vs)) / 3;
const int nDividerIs = sizeof(divider_is) / sizeof(typeof(*divider_is));
Shader* div_shader;
VertexArrayIndex* div_va;

// Viewing Frustum
Object* cam1_frustum;

// Scene
Shader* cam1_shader;
Shader* cam2_shader;
Camera* cam1;
Camera* cam2;
Scene* scene;

// Keyboard Control
Camera* focused_cam;

void exitProgram(int signal) {
  delete cam1;
  delete cam2;
  delete cam1_frustum;
  delete scene;
  cam1_shader->free();
  cam2_shader->free();
  div_shader->free();
  div_va->free();
  delete cam1_shader;
  delete cam2_shader;
  delete div_shader;
  delete div_va;
  exit(signal);
}

void processKey(unsigned char key, int x, int y) {
  switch (key) {
    case 'x':
      focused_cam->translate(Vec3(tstep, 0, 0));
      glutPostRedisplay();
      break;
    case 'y':
      focused_cam->translate(Vec3(0, tstep, 0));
      glutPostRedisplay();
      break;
    case 'z':
      focused_cam->translate(Vec3(0, 0, tstep));
      glutPostRedisplay();
      break;
    case 't':
      focused_cam->tilt(rstep);
      glutPostRedisplay();
      break;
    case 'p':
      focused_cam->pan(rstep);
      glutPostRedisplay();
      break;
    case 'r':
      focused_cam->roll(rstep);
      glutPostRedisplay();
      break;
    case 'o':
      focused_cam->zoom(fstep);
      glutPostRedisplay();
      break;
    case 'i':
      tstep = -tstep;
      rstep = -rstep;
      fstep = 1.0 / fstep;
      break;
    case 'w':
      focused_cam->translate(-focused_cam->get_dir());
      glutPostRedisplay();
      break;
    case 's':
      focused_cam->translate(focused_cam->get_dir());
      glutPostRedisplay();
      break;
    case 'a':
      focused_cam->translate(-focused_cam->get_right());
      glutPostRedisplay();
      break;
    case 'd':
      focused_cam->translate(focused_cam->get_right());
      glutPostRedisplay();
      break;
    case 'q':
      exitProgram(0);
  }
}

void menu(int option) {
  switch (option) {
    case 1:
      cam1->setObjectDisplayMode(Camera::WIREFRAME);
      cam2->setObjectDisplayMode(Camera::WIREFRAME);
      glutChangeToMenuEntry(1, "Solid", 2);
      break;
    case 2:
      cam1->setObjectDisplayMode(Camera::SOLID);
      cam2->setObjectDisplayMode(Camera::SOLID);
      glutChangeToMenuEntry(1, "Wireframe", 1);
      break;
    case 3:
      cam1->setProjectionType(Camera::ORTHO);
      cam2->setProjectionType(Camera::ORTHO);
      glutChangeToMenuEntry(2, "Perspective", 4);
      break;
    case 4:
      cam1->setProjectionType(Camera::PERSPECTIVE);
      cam2->setProjectionType(Camera::PERSPECTIVE);
      glutChangeToMenuEntry(2, "Orthographic", 3);
      break;
    case 5:
      focused_cam = cam2;
      glutChangeToMenuEntry(3, "Keyboard Ctrl Cam 1", 6);
      break;
    case 6:
      focused_cam = cam1;
      glutChangeToMenuEntry(3, "Keyboard Ctrl Cam 2", 5);
      break;
    case 7:
      cam2->lookat(Vec3(0.0, 0.0, 60.0), Vec3(0.0, 0.0, 0.0), Vec3(-1.0, 0.0, 0.0));
      break;
    case 8:
      cam2->lookat(Vec3(0.0, 0.0, -60.0), Vec3(0.0, 0.0, 0.0), Vec3(-1.0, 0.0, 0.0));
      break;
    case 9:
      cam2->lookat(Vec3(0.0, 60.0, 0.0), Vec3(0.0, 0.0, 0.0), Vec3(0.0, 0.0, 1.0));
      break;
    case 10:
      cam2->lookat(Vec3(0.0, -60.0, 0.0), Vec3(0.0, 0.0, 0.0), Vec3(0.0, 0.0, 1.0));
      break;
    case 11:
      cam2->lookat(Vec3(60.0, 0.0, 0.0), Vec3(0.0, 0.0, 0.0), Vec3(0.0, 0.0, 1.0));
      break;
    case 12:
      cam2->lookat(Vec3(-60.0, 0.0, 0.0), Vec3(0.0, 0.0, 0.0), Vec3(0.0, 0.0, 1.0));
      break;
  }
  glutPostRedisplay();
}

void display() {
  glClearColor(0.529, 0.808, 0.922, 1.0);
  glEnable(GL_DEPTH_TEST);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  
  const int w = glutGet(GLUT_WINDOW_WIDTH);
  const int h = glutGet(GLUT_WINDOW_HEIGHT);
  const int divW = 20;
  const int lPaneX = 0;
  const int lPaneW = w / 2 - divW / 2;
  const int rPaneX = lPaneX + lPaneW + divW;
  const int rPaneW = w - rPaneX;

  glViewport(lPaneX, 0, lPaneW, h);
  cam1->render(*scene);

  glViewport(rPaneX, 0, rPaneW, h);
  cam2->render(*scene);

  cam1->genFrustumVertices(*cam1_frustum);
  glEnable(GL_BLEND);
  glBlendColor(0.0, 0.0, 0.0, 0.6);
  glBlendFunc(GL_CONSTANT_ALPHA, GL_ONE_MINUS_CONSTANT_ALPHA);
  cam2->render(*cam1_frustum);
  glDisable(GL_BLEND);

  div_va->use();
  div_shader->use();

  glViewport(w / 2 - 1, 0, 2, h);
  div_shader->setVec3("color", 0.0, 0.0, 0.0);
  glDrawElements(GL_TRIANGLES, nDividerIs, GL_UNSIGNED_INT, 0);

  glViewport(w / 2 - divW / 2, 0, divW, h);
  div_shader->setVec3("color", 1.0, 1.0, 1.0);
  glDrawElements(GL_TRIANGLES, nDividerIs, GL_UNSIGNED_INT, 0);
  div_shader->unuse();
  
  glFlush();
  glutSwapBuffers();
}

void initGLUT(int* argc, char** argv) {
  if (*argc != 3) {
    std::cout << "Usage: ps4 scene-file camera-file" << std::endl;
    return;
  }

  glutInit(argc, argv);
  #ifdef __APPLE__
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH | GLUT_3_2_CORE_PROFILE);
  #else
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
  #endif
  glutInitWindowSize(SCR_WIDTH, SCR_HEIGHT);
  glutCreateWindow("ECE 30874 PS4 - drasheed");
  if (glewInit() != GLEW_OK) exit(0);

  // initialize objects
  try {
    // Divider
    div_shader = new Shader("resources/divider_shader.vs", "resources/divider_shader.fs");
    div_va = new VertexArrayIndex(nDividerVerts, divider_vs, nDividerIs, divider_is);

    // Cameras
    cam1_shader = new Shader("resources/shader.vs", "resources/shader.fs");
    cam2_shader = new Shader("resources/shader.vs", "resources/shader.fs");
    cam1 = new Camera(argv[2], (float) PANE_WIDTH / PANE_HEIGHT, *cam1_shader);
    cam2 = new Camera(45.0, (float) PANE_WIDTH / PANE_HEIGHT, 60, 200, *cam2_shader);
    cam2->lookat(Vec3(0.0, 0.0, 120.0), Vec3(0.0, 0.0, 0.0), Vec3(-1.0, 0.0, 0.0));

    // Scene
    scene = new Scene();
    scene->readFromFile(argv[1]);

    // Viewing Frustum
    cam1_frustum = new Object();
    cam1_frustum->setTransformationMatrix(Mat3(
      1, 0, 0,
      0, 1, 0,
      0, 0, 1
    ), Vec3(0, 0, 0));
    cam1_frustum->setColor(Vec3(0.2, 0.2, 0.2));

    // Camera Focus
    focused_cam = cam1;
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
  glutAddMenuEntry("Wireframe", 1);
  glutAddMenuEntry("Orthographic", 3);
  glutAddMenuEntry("Keyboard Ctrl Camera 2", 5);
  glutAddMenuEntry("Cam 2 +Z", 7);
  glutAddMenuEntry("Cam 2 -Z", 8);
  glutAddMenuEntry("Cam 2 +Y", 9);
  glutAddMenuEntry("Cam 2 -Y", 10);
  glutAddMenuEntry("Cam 2 +X", 11);
  glutAddMenuEntry("Cam 2 -X", 12);
  glutAttachMenu(GLUT_RIGHT_BUTTON);

  glutMainLoop();
}

int main(int argc, char** argv) {
  initGLUT(&argc, argv);
  return 0;
}
