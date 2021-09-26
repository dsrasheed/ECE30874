#include <iostream>
#include <math.h>
#include <stdlib.h>
#include <vector>

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
#include "Point.h"
#include "Ball.h"
#include "Polygon.h"
#include "collisions.h"

// Settings
#define SCR_WIDTH  800
#define SCR_HEIGHT 800
#define FPS        30
#define BALL_FRAC_ERROR 0.01
#define GRAVITY    9.8
#define TIMESTEP   1.0 / FPS

// Polygon Data
enum PolyState {
  POLY_NONE, POLY_DRAW, POLY_DONE
};
std::vector<Point> poly_pts;
std::vector<LineSegment> poly_lines;
Polygon polygon;
PolyState poly_state;

// Ball Data
enum BallState {
  BALL_NONE, BALL_DRAW, BALL_DONE
};
BallState ball_state;
Vector acc = Vector(0, GRAVITY);
Vector ball_velocity;
Ball ball;

// Program Data
enum PS2_State {
  NOT_READY, READY, ANIMATING
};
PS2_State state;

void exitProgram(int signal) {
  ball.free();
  polygon.free();
  std::vector<Point>().swap(poly_pts);
  std::vector<LineSegment>().swap(poly_lines);
  exit(signal);
}

void display(void) {
  glClear(GL_COLOR_BUFFER_BIT);
  
  if (ball_state == BALL_DONE) {
    ball.draw();
  }

  if (poly_state == POLY_DRAW) {
    polygon.setDrawMode(OPEN);
    polygon.draw();
  }

  if (poly_state == POLY_DONE) {
    polygon.setDrawMode(CLOSED);
    polygon.draw();
  }

  glFlush();
  glutSwapBuffers();
}

bool inPolygon(Ray r) {
  int nIntersections = 0;
  for (int i = 0; i < poly_lines.size(); i++) {
    Point p = intersection(r, poly_lines[i]);
    if (!p.invalid) {
      nIntersections++;
    }
  }

  // If odd number of intersection points, ball is outside of polygon
  return nIntersections % 2 == 1;
}

void animate(int value) {
  if (state != ANIMATING)
      return;
    
  // Determine what lines the path of the ball intersects with
  std::vector<Point> intersections;
  std::vector<int> line_indices;
  for (int i = 0; i < poly_lines.size(); i++) {
    Point p = intersection(Ray(ball.getCenter(), ball_velocity * TIMESTEP), poly_lines[i]);
    if (!p.invalid) {
      intersections.push_back(p);
      line_indices.push_back(i);
    }
  }

  // If even number of intersection points, ball is outside of polygon
  if (intersections.size() % 2 == 0 && !ball_velocity.isZeroVector()) {
    state = READY;
    return;
  }

  /*if (!inPolygon(Ray(ball.getCenter(), ball_velocity * TIMESTEP)) && !ball_velocity.isZeroVector()) {
    state = READY;
    return;
  }*/

  // Determine the closest line that intersects with the path of the ball.
  float smallest_norm = 40000000;
  int line_index = -1;
  for (int i = 0; i < intersections.size(); i++) {
    Point p = intersections[i];
    Vector v =  p - ball.getCenter();
    //float norm = abs(v.dot(poly_lines[line_indices[i]].vector.normalVector()));
    float norm = v.norm();
    if (norm < smallest_norm) {
      smallest_norm = norm;
      line_index = line_indices[i];
    }
  }

  // If the ball has collided with that line, calculate the ball's new velocity
  LineSegment closestLine = poly_lines[line_index];
  Vector n0 = closestLine.vector.normalVector();
  float dist = ((ball_velocity - acc * TIMESTEP) * TIMESTEP).norm();
  if (smallest_norm <= (ball.getRadius() + 0.01)) {
    ball_velocity = ball_velocity - (n0 * (n0.dot(ball_velocity))) * 2;
    ball.setCenter(ball.getCenter() + ball_velocity * TIMESTEP);
  }
  else if (dist - smallest_norm > 0) {
    ball.setCenter(ball.getCenter() + (ball_velocity * (1 / ball_velocity.norm())) * (smallest_norm - ball.getRadius()));
  }
  else {
    ball_velocity = ball_velocity - acc * TIMESTEP;
    ball.setCenter(ball.getCenter() + ball_velocity * TIMESTEP);
  }

  /*bool intersection = false;
  for (int i = 0; i < poly_lines.size(); i++) {
    LineSegment segment = poly_lines[i];
    Vector v = segment.point - ball.getCenter();
    Vector norm = segment.vector.normalVector();
    float perp_dist = v.dot(norm);

    // If on the next time step, you will be outside that line, you need
    // to collide with it now. Calculate the projection on the norm. Before and after
    // applying the velocity. Add the two norms to each other. if the new norm is less than
    // either of the norms, then the ball is outside the line.
    Point p1 = ball.getCenter() + (ball_velocity - acc * TIMESTEP) * TIMESTEP;
    Vector v1 = segment.point - p1;
    float dist2 = v1.dot(norm);

    bool willcross = !inPolygon(Ray(ball.getCenter() + ((ball_velocity - (acc * TIMESTEP)) * TIMESTEP), ball_velocity - (acc * TIMESTEP)));
    std::cout << willcross << std::endl;
    if ((abs(perp_dist) <= ball.getRadius()) || willcross) {
      std::cout << "INDEX: " << i << std::endl;
      std::cout << "IN HERE" << std::endl << "NORM: ";
      norm.print();
      std::cout << std::endl;
      std::cout << "DOT: " << norm.dot(ball_velocity) << std::endl;
      ball_velocity = ball_velocity - norm * (norm.dot(ball_velocity)) * 2;
      ball_velocity.print();
      ball.setCenter(ball.getCenter() + ball_velocity * TIMESTEP);
      std::cout << "END" << std::endl;
      intersection = true;
      break;
    }
  }*/
  

  std::vector<Point>().swap(intersections);
  std::vector<int>().swap(line_indices);
  
  /*Vector acc = Vector(0, GRAVITY);
  std::cout << "Before Update: ";
  ball_velocity.print();
  std::cout << std::endl;

  ball_velocity = ball_velocity - acc * TIMESTEP;
  std::cout << "After Update: ";
  ball_velocity.print();
  std::cout << std::endl;*/
  /*if (!intersection) {
      ball_velocity = ball_velocity - acc * TIMESTEP;
      ball.setCenter(ball.getCenter() + ball_velocity * TIMESTEP);
  }*/

  glutPostRedisplay();
  glutTimerFunc(seconds2millis(TIMESTEP), animate, 0);
}

void deletePolygon() {
  if (poly_state != POLY_DONE)
    return;

  poly_pts.clear();
  poly_lines.clear();
  polygon.free();
  poly_state = POLY_NONE;
  glutPostRedisplay();
}

bool isReady() {
  return poly_state == POLY_DONE && ball_state == BALL_DONE;
}

void polyDrawMouseHandler(int btn, int btn_state, int x, int y) {
  if (poly_state != POLY_DRAW)
    return;

  if (btn_state != GLUT_UP)
    return;

  switch (btn) {
    case GLUT_LEFT_BUTTON:
      poly_pts.push_back(Point(x, y).normalize());
      break;
    case GLUT_MIDDLE_BUTTON:
      if (!poly_pts.empty()) {
        poly_pts.pop_back();
      }
      break;
    case GLUT_RIGHT_BUTTON:
      poly_state = POLY_DONE;
      poly_pts.push_back(Point(x, y).normalize());
      LineSegment::genLineSegmentsFromPoints(&poly_lines, poly_pts);
      break;
  }

  float* vertices;
  Point::genVerticesFromPoints(&vertices, poly_pts);
  if (!polygon.invalid) {
    polygon.free();
  }
  polygon = Polygon(poly_pts.size(), vertices);
  glutPostRedisplay();
}

void ballDrawMouseHandler(int btn, int btn_state, int x, int y) {
  if (ball_state != BALL_DRAW)
    return;

  if (btn != GLUT_LEFT_BUTTON)
    return;

  switch (btn_state) {
    case GLUT_DOWN:
      ball.setCenter(Point(x, y).normalize());
      break;
    case GLUT_UP:
      ball.setRadiusUsingPointOnPerimeter(Point(x, y).normalize());
      ball_state = BALL_DONE;
      glutPostRedisplay();
  }
}

void ballPlaceMouseHandler(int btn, int btn_state, int x, int y) {
  if (state != READY && state != ANIMATING)
    return;
  
  if (btn_state != GLUT_UP)
    return;
  
  ball_velocity = Vector(0,0);
  ball.setCenter(Point(x, y).normalize());
  // if it's animating, glutPostRedisplay will be called on the next tick, so don't call it
  if (state != ANIMATING) {
    glutPostRedisplay();
  }
}

void processMouse(int btn, int btn_state, int x, int y) {
  polyDrawMouseHandler(btn, btn_state, x, y);
  ballDrawMouseHandler(btn, btn_state, x, y);
  ballPlaceMouseHandler(btn, btn_state, x, y);
  if (isReady() && state == NOT_READY) {
    state = READY;
  }
}

void processKey(unsigned char key, int x, int y) {
  switch (key) {
    case 'p':
      if (state != ANIMATING && poly_state != POLY_DRAW && ball_state != BALL_DRAW) {
        deletePolygon();
        state = NOT_READY;
        poly_state = POLY_DRAW;
      }
      break;
    case 'b':
      if (state != ANIMATING && poly_state != POLY_DRAW && ball_state != BALL_DRAW) {
        state = NOT_READY;
        ball_state = BALL_DRAW;
        ball_velocity = Vector(0,0);
        glutPostRedisplay();
      }
      break;
    case 'a':
      if (state == READY) {
          state = ANIMATING;
          glutTimerFunc(seconds2millis(TIMESTEP), animate, 0);
      }
      break;
    case 's':
      if (state == ANIMATING) {
        state = READY;
      }
      break;
    case 'q':
      exitProgram(0);
  }
  /*std::cout << "Poly State   : " << poly_state << std::endl;
  std::cout << "Ball State   : " << ball_state << std::endl;
  std::cout << "Program State: " << state << std::endl;*/
}

void initGLUT(int* argc, char** argv) {
  glutInit(argc, argv);
  #ifdef __APPLE__
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_3_2_CORE_PROFILE);
  #else
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
  #endif
  glutInitWindowSize(SCR_WIDTH, SCR_HEIGHT);
  glutCreateWindow("example window");
  if (glewInit() != GLEW_OK) exit(0);

  // initialize program state
  poly_state = POLY_NONE;
  ball_state = BALL_NONE;
  ball = Ball(BALL_FRAC_ERROR);
  state = NOT_READY;

  // callbacks
  glutDisplayFunc(display);
  glutKeyboardUpFunc(processKey);
  glutMouseFunc(processMouse);

  glutMainLoop();
}

int main(int argc, char** argv) {
  initGLUT(&argc, argv);
  return 0;
}
