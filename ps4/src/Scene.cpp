#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <string.h>

#include "Object.h"
#include "Scene.h"
#include "Mat3.h"

#define MAX_FILENAME_LEN 260

Scene::Scene() {
    objects = nullptr;
}

void Scene::readFromFile(const char* filename) {
    std::ifstream f(filename);
    if (f.is_open()) {
        f >> nObjs;

        this->objects = new Object[nObjs];
        char filename[MAX_FILENAME_LEN];
        Mat3 fn1;
        Vec3 tr1;
        Vec3 color;
        try {
            for (int i = 0; i < nObjs; i++) {
                do {
                    f.getline(filename, MAX_FILENAME_LEN);
                } while (strlen(filename) == 0);
                f >> fn1 >> tr1 >> color;
                
                this->objects[i].readFromFile(filename);
                this->objects[i].setTransformationMatrix(fn1, tr1);
                this->objects[i].setColor(color);
            }
        } catch (const std::string s) {
            delete[] this->objects;
            this->objects = nullptr;
            nObjs = 0;
            throw s;
        }
    } else {
        std::stringstream ss;
        ss << "Unable to open scene file " << filename;
        throw ss.str();
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