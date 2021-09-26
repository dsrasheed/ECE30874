#include <iostream>
#include <fstream>
#include <string>
#include <string.h>

#include "Object.h"
#include "Scene.h"
#include "Mat3.h"

Scene::Scene() {
    objects = nullptr;
}

void Scene::readFromFile(const char* filename) {
    std::ifstream f(filename);
    if (f.is_open()) {
        f >> nObjs;

        this->objects = new Object[nObjs];
        char filename[260];
        Mat3 fn1;
        Vec3 tr1;
        Vec3 color;
        try {
            for (int i = 0; i < nObjs; i++) {
                do {
                    f.getline(filename, 260);
                } while (strlen(filename) == 0);
                f >> fn1 >> tr1 >> color;
                
                this->objects[i].readFromFile(filename);
                this->objects[i].setTransformationMatrix(fn1, tr1);
                this->objects[i].setColor(color);
            }
        } catch (const std::string s) {
            std::cout << s << std::endl;
            delete[] this->objects;
            this->objects = nullptr;
            nObjs = 0;
        }
    } else {
        throw "Unable to open file";
    }
    f.close();
}

std::ostream& operator<<(std::ostream& os, const Scene& s) {
    for (int i = 0; i < s.nObjs; i++) {
        os << "Object " << i << std::endl;
        os << s.objects[i];
    }
    return os;
}

int Scene::getNumObjects() const {
    return nObjs;
}

const Object* Scene::getObjects() const {
    return objects;
}

Scene::~Scene() {
    if (objects != nullptr) {
        delete[] objects;
        objects = nullptr;
    }
}