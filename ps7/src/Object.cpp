#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <sstream>

#include "Vec3.h"
#include "Object.h"

Object::Object() {
    vertices = nullptr;
    indices = nullptr;
    smooth_verts = nullptr;
    flat_verts = nullptr;
    smoothVa = nullptr;
    flatVa = nullptr;
    tx = nullptr;
    _hasTx = 0;
    nverts = 0;
    ntris = 0;
}

Object::Object(const char* filename) {
    vertices = nullptr;
    indices = nullptr;
    smooth_verts = nullptr;
    flat_verts = nullptr;
    smoothVa = nullptr;
    flatVa = nullptr;
    tx = nullptr;
    nverts = 0;
    ntris = 0;
    _hasTx = 0;
    readFromFile(filename);
}

void Object::readFromFile(const char* filename) {
    reset();

    std::ifstream f(filename);
    if (f.is_open()) {
        f >> nverts;
        f >> ntris;
        f >> _hasTx;

        vertices = new float[nverts * 5];
        for (int i = 0; i < nverts * 5; i++) {
            if (!_hasTx && (i % 5 > 2)) {
                vertices[i] = 0;
            }
            else {
                f >> vertices[i];
            }
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

    vertices = new float[nverts * 5];
    for (int i = 0; i < nverts * 5; i++) {
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
        float* vA = &vertices[indices[i*3+0]*5];
        float* vB = &vertices[indices[i*3+1]*5];
        float* vC = &vertices[indices[i*3+2]*5];
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
    smooth_verts = new float[nSmoothVerts * 8];
    unsigned int nFlatVerts = ntris * 3;
    flat_verts = new float[nFlatVerts * 8];
    for (int i = 0; i < ntris; i++) {
        for (int j = 0; j < 3; j++) {
            unsigned int vIndex = indices[i*3+j];
            unsigned int base = i*8*3 + j*8;
            smooth_verts[base+0] = vertices[vIndex*5+0];
            smooth_verts[base+1] = vertices[vIndex*5+1];
            smooth_verts[base+2] = vertices[vIndex*5+2];
            smooth_verts[base+3] = vertex_normals[vIndex].x;
            smooth_verts[base+4] = vertex_normals[vIndex].y;
            smooth_verts[base+5] = vertex_normals[vIndex].z;
            flat_verts[base+0] = vertices[vIndex*5+0];
            flat_verts[base+1] = vertices[vIndex*5+1];
            flat_verts[base+2] = vertices[vIndex*5+2];
            flat_verts[base+3] = face_normals[i].x;
            flat_verts[base+4] = face_normals[i].y;
            flat_verts[base+5] = face_normals[i].z;
            if (_hasTx) {
                smooth_verts[base+6] = vertices[vIndex*5+3];
                smooth_verts[base+7] = vertices[vIndex*5+4];
                flat_verts[base+6] = vertices[vIndex*5+3];
                flat_verts[base+7] = vertices[vIndex*5+4];
            }
            else {
                smooth_verts[base+6] = 0;
                smooth_verts[base+7] = 0;
                flat_verts[base+6] = 0;
                flat_verts[base+7] = 0;
            }
        }
    }

    smoothVa = new VertexArray8(nSmoothVerts, smooth_verts);
    flatVa   = new VertexArray8(nFlatVerts, flat_verts);

    delete[] face_normals;
    delete[] vertex_normals;
}

void Object::setMaterial(const Material& m) {
    this->m = m;
}

void Object::setTexture(Texture* tx) {
    if (this->tx != nullptr) this->tx->free();
    this->tx = tx;
    _hasTx = 1;
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

    if (obj._hasTx) 
    {
    o << "Texture" << std::endl;
    o << "-------" << std::endl;
    o << *obj.tx;
    o << std::endl;
    }

    o << "# Vertices: " << obj.nverts << std::endl;
    o << "----------------" << std::endl;
    for (int i = 0; i < obj.nverts * 5; i++) {
        if (i % 5 == 0) o << "(";
        o << obj.vertices[i];
        if (i % 5 < 4) o << ", ";
        else o << ")" << std::endl;
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

/*Object& Object::operator=(const Object& other) {
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
}*/

unsigned int Object::getNumVertices() const {
    return nverts;
}

unsigned int Object::getNumTriangles() const {
    return ntris;
}

/*const float* Object::getVertices() const {
    return vertices;
}*/

const float* Object::getSmoothVerts() const {
    return smooth_verts;
}

const float* Object::getFlatVerts() const {
    return flat_verts;
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

const Texture& Object::getTexture() const {
    if (!_hasTx) throw "Texture not initialized";
    return *tx;
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

const bool Object::hasTx() const {
    return _hasTx;
}

Object::~Object() {
    reset();
}

void Object::reset() {
    if (vertices != nullptr) delete[] vertices;
    if (indices  != nullptr) delete[] indices;
    if (smooth_verts != nullptr) delete[] smooth_verts;
    if (flat_verts != nullptr) delete[] flat_verts;
    if (smoothVa != nullptr) {
        smoothVa->free();
        delete smoothVa;
    }
    if (flatVa != nullptr) {
        flatVa->free();
        delete flatVa;
    }
    if (_hasTx) {
        tx->free();
        delete tx;
        tx = nullptr;
    }
    _hasTx = 0;
    smooth_verts = nullptr;
    flat_verts = nullptr;
    vertices = nullptr;
    indices = nullptr;
    smoothVa = nullptr;
    flatVa = nullptr;
    tx = nullptr;
    nverts = 0;
    ntris = 0;
}