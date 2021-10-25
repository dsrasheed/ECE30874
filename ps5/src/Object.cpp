#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <sstream>

#include "Vec3.h"
#include "Object.h"

Object::Object() {
    vertices = nullptr;
    indices = nullptr;
    smoothVa = nullptr;
    flatVa = nullptr;
    nverts = 0;
    ntris = 0;
}

Object::Object(const char* filename) {
    vertices = nullptr;
    indices = nullptr;
    smoothVa = nullptr;
    flatVa = nullptr;
    nverts = 0;
    ntris = 0;
    readFromFile(filename);
}

void Object::readFromFile(const char* filename) {
    reset();

    std::ifstream f(filename);
    int random0;
    if (f.is_open()) {
        f >> nverts;
        f >> ntris;
        f >> random0;

        vertices = new float[nverts * 3];
        for (int i = 0; i < nverts * 3; i++) {
            f >> vertices[i];
        }

        indices = new unsigned int[ntris * 3];
        for (int i = 0; i < ntris * 3; i++) {
            f >> indices[i];
        }
        genSmoothAndFlatVertices();
    }
    else {
        std::stringstream ss;
        ss << "Unable to open object file " << filename;
        throw ss.str();
    }
    f.close();
}

void Object::setVertices(int nverts, const float* verts, int ntris, const unsigned int* tris) {
    reset();

    this->nverts = nverts;
    this->ntris = ntris;

    vertices = new float[nverts * 3];
    for (int i = 0; i < nverts * 3; i++) {
        vertices[i] = verts[i];
    }

    indices = new unsigned int[ntris * 3];
    for (int i = 0; i < ntris * 3; i++) {
        indices[i] = tris[i];
    }

    genSmoothAndFlatVertices();
}

void Object::genSmoothAndFlatVertices() {
    if (smoothVa != nullptr || flatVa != nullptr) {
        throw "Reset the object before generating smooth and flat vertices";
    }

    Vec3* face_normals = new Vec3[ntris];
    for (int i = 0; i < ntris; i++) {
        float* vA = &vertices[indices[i*3+0]*3];
        float* vB = &vertices[indices[i*3+1]*3];
        float* vC = &vertices[indices[i*3+2]*3];
        Vec3 u = Vec3(vB[0] - vA[0], vB[1] - vA[1], vB[2] - vA[2]);
        Vec3 v = Vec3(vC[0] - vA[0], vC[1] - vA[1], vC[2] - vA[2]);
        Vec3 n = u.cross(v);
        face_normals[i] = n;
    }

    Vec3* vertex_normals = new Vec3[nverts];
    for (int i = 0; i < nverts; i++) {
        Vec3 n;
        for (int j = 0; j < ntris; j++) {
            if (indices[j*3+0] == i || indices[j*3+1] == i || indices[j*3+2] == i) {
                n += face_normals[j].normalize() / face_normals[j].norm() / 2;
            }
        }
        vertex_normals[i] = n;
    }

    unsigned int nSmoothVerts = ntris * 3;
    float* smooth_verts = new float[nSmoothVerts * 6];
    unsigned int nFlatVerts = ntris * 3;
    float* flat_verts = new float[nFlatVerts * 6];
    for (int i = 0; i < ntris; i++) {
        for (int j = 0; j < 3; j++) {
            unsigned int vIndex = indices[i*3+j];
            unsigned int base = i*6*3 + j*6;
            smooth_verts[base+0] = vertices[vIndex*3+0];
            smooth_verts[base+1] = vertices[vIndex*3+1];
            smooth_verts[base+2] = vertices[vIndex*3+2];
            smooth_verts[base+3] = vertex_normals[vIndex].x;
            smooth_verts[base+4] = vertex_normals[vIndex].y;
            smooth_verts[base+5] = vertex_normals[vIndex].z;
            flat_verts[base+0] = vertices[vIndex*3+0];
            flat_verts[base+1] = vertices[vIndex*3+1];
            flat_verts[base+2] = vertices[vIndex*3+2];
            flat_verts[base+3] = face_normals[i].x;
            flat_verts[base+4] = face_normals[i].y;
            flat_verts[base+5] = face_normals[i].z;
        }
    }

    smoothVa = new VertexArray(nSmoothVerts, smooth_verts);
    flatVa = new VertexArray(nFlatVerts, flat_verts);

    /*std::cout << "Smooth VA" << std::endl;
    std::cout << "=========" << std::endl;
    print_verts(nSmoothVerts, smooth_verts);
    std::cout << "Flat VA" << std::endl;
    std::cout << "=========" << std::endl;
    print_verts(nFlatVerts, flat_verts);*/

    delete[] face_normals;
    delete[] vertex_normals;
    delete[] smooth_verts;
    delete[] flat_verts;
}

void Object::setMaterial(const Material& m) {
    this->m = m;
}

void Object::setTransformationMatrix(const Mat3& m, const Vec3& v) {
    this->fn = m;
    this->tr = v;
}

std::ostream& operator<<(std::ostream& o, const Object& obj) {
    o << "===========================================================" << std::endl;

    o << "Material" << std::endl;
    o << "--------" << std::endl;
    o << "Diffuse:   " << obj.m.d << std::endl;
    o << "Specular:  " << obj.m.s << std::endl;
    o << "Shininess: " << obj.m.shininess << std::endl;
    o << std::endl;

    o << "Transformation Matrix" << std::endl;
    o << "---------------------" << std::endl;
    o << obj.fn << std::endl;
    o << obj.tr << std::endl;
    o << std::endl;

    o << "# Vertices: " << obj.nverts << std::endl;
    o << "----------------" << std::endl;
    for (int i = 0; i < obj.nverts * 3; i += 3) {
        o << "(" << obj.vertices[i] << ", " << obj.vertices[i+1] << ", " << obj.vertices[i+2] << ")" << std::endl;
    }
    o << std::endl;

    o << "# Triangles: " << obj.ntris << std::endl;
    o << "-----------------" << std::endl;
    for (int i = 0; i < obj.ntris * 3; i += 3) {
        o << "(" << obj.indices[i] << ", " << obj.indices[i+1] << ", " << obj.indices[i+2] << ")" << std::endl;
    }
    o << std::endl;

    o << "===========================================================" << std::endl;
    return o;
}

Object& Object::operator=(const Object& other) {
    reset();

    if (other.vertices != nullptr && other.indices != nullptr) {
        this->vertices = new float[other.nverts * 3];
        this->nverts = other.nverts;
        for (int i = 0; i < nverts * 3; i++) {
            this->vertices[i] = other.vertices[i];
        }

        this->indices = new unsigned int[other.ntris * 3];
        this->ntris = other.ntris;
        for (int i = 0; i < ntris * 3; i++) {
            this->indices[i] = other.indices[i];
        }

        genSmoothAndFlatVertices();
    }
    return *this;
}

unsigned int Object::getNumVertices() const {
    return nverts;
}

unsigned int Object::getNumTriangles() const {
    return ntris;
}

const float* Object::getVertices() const {
    return vertices;
}

const unsigned int* Object::getTriangles() const {
    return indices;
}

const Mat3& Object::getTransformMatrix() const {
    return fn;
}

const Vec3& Object::getTranslationVector() const {
    return tr;
}

const Material& Object::getMaterial() const {
    return m;
}

const VertexArray& Object::getSmoothVa() const {
    return *smoothVa;
}

const VertexArray& Object::getFlatVa() const {
    return *flatVa;
}

Object::~Object() {
    reset();
}

void Object::reset() {
    if (vertices != nullptr) delete[] vertices;
    if (indices  != nullptr) delete[] indices;
    if (smoothVa != nullptr) {
        smoothVa->free();
        delete smoothVa;
    }
    if (flatVa != nullptr) {
        flatVa->free();
        delete flatVa;
    }
    vertices = nullptr;
    indices = nullptr;
    smoothVa = nullptr;
    flatVa = nullptr;
    nverts = 0;
    ntris = 0;
}