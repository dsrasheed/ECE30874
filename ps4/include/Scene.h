#ifndef __ECE30874_SCENE_H__
#define __ECE30874_SCENE_H__

#include <iostream>
#include "Object.h"

class Scene {
    private:
        Object* objects;
        int nObjs;
    public:
        Scene();
        void readFromFile(const char* filename);
        friend std::ostream& operator<<(std::ostream& os, const Scene& s);

        int getNumObjects() const;
        const Object* getObjects() const;
        
        ~Scene();
};

#endif