#ifndef __ECE30874_SCENE_H__
#define __ECE30874_SCENE_H__

#include <iostream>
#include "Object.h"
#include "Vec3.h"

typedef struct Light {
    Vec3 a, d, s, p;
    bool dir;
    float kl, kq; 
} Light;

class Scene {
    private:
        Object* objects;
        Light* lights;
        int nObjs, nLights;
    public:
        Scene();
        void readFromFile(const char* filename);
        friend std::ostream& operator<<(std::ostream& os, const Scene& s);

        int getNumObjects() const;
        int getNumLights() const;
        const Object* getObjects() const;
        Light* getLights() const;
        
        ~Scene();
};

#endif