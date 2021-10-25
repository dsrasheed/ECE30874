#ifndef __ECE30874_OBJECT_H__
#define __ECE30874_OBJECT_H__

#include "Vec3.h"
#include "Mat3.h"
#include "VertexArray.h"

typedef struct Material {
    Vec3 d, s;
    float shininess;
} Material;

class Object {
    private:
        float* vertices;
        unsigned int nverts;
        unsigned int* indices;
        unsigned int ntris;
        Mat3 fn;
        Vec3 tr;
        Material m;
        VertexArray* smoothVa;
        VertexArray* flatVa;

        void reset();
        void genSmoothAndFlatVertices();

    public:

        Object();

        Object(const char* filename);

        void readFromFile(const char* filename);

        void setVertices(int nverts, const float* verts, int ntris, const unsigned int* tris);

        void setMaterial(const Material& mat);

        void setTransformationMatrix(const Mat3& m, const Vec3& v);

        friend std::ostream& operator<<(std::ostream& o, const Object& obj);

        Object& operator=(const Object& o);

        unsigned int getNumVertices() const;
        unsigned int getNumTriangles() const;
        const float* getVertices() const;
        const unsigned int* getTriangles() const;

        const Mat3& getTransformMatrix() const;
        const Vec3& getTranslationVector() const;
        const Material& getMaterial() const;
        const VertexArray& getSmoothVa() const;
        const VertexArray& getFlatVa() const;

        ~Object();
};

#endif