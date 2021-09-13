#ifdef __APPLE__
#define GL_SILENCE_DEPRECATION
#include <OpenGL/gl.h>
#include <GLUT/glut.h>
#else
// TODO: Ask what headers should be for them
#include <gl.h>
#include <glut.h>
#endif

void renderScene(void) {
  glClear(GL_COLOR_BUFFER_BIT);
  glDrawArrays(GL_TRIANGLES, 0, 3);
  glFlush();
  glutSwapBuffers();
}

int main(int argc, char** argv) {
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
  glutInitWindowSize(SCR_WIDTH, SCR_HEIGHT);
  glutCreateWindow("Example Window");

  glutDisplayFunc(renderScene);

  glutMainLoop();

  return 0;
}
