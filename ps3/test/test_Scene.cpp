#include <iostream>
#include "Scene.h"

int main() {
    Scene s;
    s.readFromFile("resources/scene1-3");
    std::cout << s;
}