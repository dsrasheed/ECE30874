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

        objects = new Object[nObjs];
        char objFile[MAX_FILENAME_LEN];
        char txFile[MAX_FILENAME_LEN];
        Mat3 fn1;
        Vec3 tr1;
        Texture* tx;
        Material m;
        try {
            for (int i = 0; i < nObjs; i++) {
                do {
                    f.getline(objFile, MAX_FILENAME_LEN);
                } while (strlen(objFile) == 0);
                f >> fn1 >> tr1;
                f >> m.d >> m.s >> m.shininess;
                
                objects[i].readFromFile(objFile);
                if (objects[i].hasTx()) {
                    do {
                        f.getline(txFile, MAX_FILENAME_LEN);
                    } while (strlen(txFile) == 0);
                    tx = new Texture(txFile);
                    objects[i].setTexture(tx);
                }
                objects[i].setTransformationMatrix(fn1, tr1);
                objects[i].setMaterial(m);
                // reset
                tx = nullptr;
            }
        } catch (const std::string s) {
            delete[] objects;
            objects = nullptr;
            nObjs = 0;
            throw s;
        }
        f >> nLights;
        lights = new Light[nLights];
        try {
            for (int i = 0; i < nLights; i++) {
                f >> lights[i].a >> lights[i].d >> lights[i].s >> lights[i].p;
                f >> lights[i].dir >> lights[i].kl >> lights[i].kq;
            }
        }
        catch (const std::string s) {
            delete[] objects;
            objects = nullptr;
            delete[] lights;
            lights = nullptr;
            nObjs = 0;
            nLights = 0;
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
    for (int i = 0; i < s.nLights; i++) {
        os << std::endl;
        os << "Light " << i << std::endl;
        os << "Ambient : " << s.lights[i].a << std::endl;
        os << "Diffuse : " << s.lights[i].d << std::endl;
        os << "Specular: " << s.lights[i].s << std::endl;
        os << "Position: " << s.lights[i].p << std::endl;
        os << "Dir     : " << s.lights[i].dir << std::endl;
        os << "KL      : " << s.lights[i].kl << std::endl;
        os << "KQ      : " << s.lights[i].kq << std::endl;
    }
    return os;
}

int Scene::getNumLights() const {
    return nLights;
}

int Scene::getNumObjects() const {
    return nObjs;
}

const Object* Scene::getObjects() const {
    return objects;
}

Light* Scene::getLights() const {
    return lights;
}

Scene::~Scene() {
    if (objects != nullptr) {
        delete[] objects;
        objects = nullptr;
    }
    if (lights != nullptr) {
        delete[] lights;
        lights = nullptr;
    }
}