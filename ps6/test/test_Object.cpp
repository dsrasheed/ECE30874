#include <iostream>
#include <cassert>

#include "Object.h"

int main() {
  Object o1("resources/test/x.obj");
  std::cout << o1;

  Object o2("resources/cube.obj");
  std::cout << o2;

  Object o3("resources/box.obj");
  std::cout << o3;

  Object o4("resources/floor.obj");
  std::cout << o4;

  Object o5("resources/pyramid.obj");
  std::cout << o5;
}
