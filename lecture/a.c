#include <stdlib.h>
#include <iostream>

#ifdef __APPLE__
#define GL_SILENCE_DEPRECATION
#include <GL/glew.h>
#include <OpenGL/gl.h>
#include <GLUT/glut.h>
#else
// TODO: Ask what headers should be for their system
#include <GL/glew.h>
#include <GL/glut.h>
#endif

#define MENU_VIEWMODE 0
#define MENU_EXIT 1

float vertices[] = {
  +0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f,
  -0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f,
  +0.0f, +0.5f, 0.0f, 0.0f, 0.0f, 1.0f
};

const char* vertexShaderSource = "#version 330 core\n"
  "layout (location = 0) in vec3 aPos;\n"
  "layout (location = 1) in vec3 aColor;\n"
  "out vec3 ourColor;\n"
  "void main() {\n"
  " gl_Position = vec4(aPos, 1.0);\n"
  " ourColor = aColor;\n"
  "}\n";

const char* fragmentShaderSource = "#version 330 core\n"
  "out vec4 FragColor;\n"
  "in vec3 ourColor;\n"
  "void main(){\n"
  " FragColor = vec4(ourColor, 1.0f);\n"
  "}\n";

void buildTriangle() {
  // build and compile shader program
  unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
  glCompileShader(vertexShader);
  // check for shader compile errors
  int success;
  char infoLog[512];
  glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
  if (!success) {
    glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
    std::cout << "ERROR::SHADER::VERTEX COMPILATION_FAILED\n" << infoLog << std::endl;
  }
  // fragment shader
  unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
  glCompileShader(fragmentShader);
  // check for shader compile errors
  glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
  if (!success) {
    glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
    std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
  }
  // link shaders
  unsigned int shaderProgram = glCreateProgram();
  glAttachShader(shaderProgram, vertexShader);
  glAttachShader(shaderProgram, fragmentShader);
  glLinkProgram(shaderProgram);
  // check for linking errors
  glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
  if (!success) {
    glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
    std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
  }
  glDeleteShader(vertexShader);
  glDeleteShader(fragmentShader);

  // Vertex Buffer Object
  unsigned int VBO, VAO;
  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &VBO);
  // bind the Vertex Array Object first, then bind and set vertex buffer(s).
  // and then configure vertex attribute(s)
  glBindVertexArray(VAO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
  // position attribute
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*) 0);
  glEnableVertexAttribArray(0);
  // color attribute
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*) (3*sizeof(float)));
  glEnableVertexAttribArray(1);

  glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT);
  glUseProgram(shaderProgram);
  glBindVertexArray(VAO);
}

void display(void) {
  glClear(GL_COLOR_BUFFER_BIT);
  glLineWidth(2.0);
  glDrawArrays(GL_TRIANGLE_FAN, 0, 3);
  glFlush();
}

void menu(int cmd) {
  switch (cmd) {
    case MENU_VIEWMODE:
      glutPostRedisplay();
      break;
    case MENU_EXIT:
      exit(0);
  }
}


void keyRelease(unsigned char key, int x, int y) {
  switch (key) {
    case 'q':
      menu(MENU_EXIT);
      break;
  }
}

void mouseBtn(int button, int state, int x, int y) {
}

void mouseMove(int x, int y) {
  glutPostRedisplay();
}

void initGLUT(int* argc, char** argv) {
  glutInit(argc, argv);
  glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB | GLUT_3_2_CORE_PROFILE);
  glutInitWindowSize(800, 600);
  glutCreateWindow("example window");
  glutDisplayFunc(display);
  if (glewInit() != GLEW_OK) exit(0);

  // build traingle
  buildTriangle();

  // Create Menu
  glutCreateMenu(menu);
  glutAddMenuEntry("Toggle view mode", MENU_VIEWMODE);
  glutAddMenuEntry("Exit", MENU_EXIT);
  glutAttachMenu(GLUT_RIGHT_BUTTON);

  // callbacks
  glutKeyboardUpFunc(keyRelease);

  glutMainLoop();
}

int main(int argc, char** argv) {
  initGLUT(&argc, argv);
  return 0;
}
