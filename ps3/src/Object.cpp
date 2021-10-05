#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <sstream>

#include "Object.h"

Object::Object() {
    vertices = nullptr;
    indices = nullptr;
}

Object::Object(const char* filename) {
    vertices = nullptr;
    indices = nullptr;
    readFromFile(filename);
}

void Object::readFromFile(const char* filename) {
    reset();

    std::ifstream f(filename);
    if (f.is_open()) {
        f >> nverts;
        f >> ntris;

        vertices = new float[nverts * 3];
        for (int i = 0; i < nverts * 3; i++) {
            f >> vertices[i];
        }

        indices = new unsigned int[ntris * 3];
        for (int i = 0; i < ntris * 3; i++) {
            f >> indices[i];
        }
    }
    else {
        std::stringstream ss;
        ss << "Unable to open object file " << filename;
        throw ss.str();
    }
    f.close();
}

void Object::setColor(const Vec3& color) {
    this->color = color;
}

void Object::setTransformationMatrix(const Mat3& m, const Vec3& v) {
    this->fn = m;
    this->tr = v;
}

std::ostream& operator<<(std::ostream& o, const Object& obj) {
    o << "===========================================================" << std::endl;

    o << "Color: " << std::endl;
    o << "--------------" << std::endl;
    o << obj.color << std::endl;
    o << std::endl;

    o << "Transformation Matrix: " << std::endl;
    o << "--------------" << std::endl;
    o << obj.fn << std::endl;
    o << obj.tr << std::endl;
    o << std::endl;

    o << "# Vertices: " << obj.nverts << std::endl;
    o << "--------------" << std::endl;
    for (int i = 0; i < obj.nverts * 3; i += 3) {
        o << "(" << obj.vertices[i] << ", " << obj.vertices[i+1] << ", " << obj.vertices[i+2] << ")" << std::endl;
    }
    o << std::endl;

    o << "# Triangles: " << obj.ntris << std::endl;
    o << "--------------" << std::endl;
    for (int i = 0; i < obj.ntris * 3; i += 3) {
        o << "(" << obj.indices[i] << ", " << obj.indices[i+1] << ", " << obj.indices[i+2] << ")" << std::endl;
    }
    o << std::endl;

    o << "===========================================================" << std::endl;
    return o;
}

Object& Object::operator=(const Object& other) {
    reset();

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

const Vec3& Object::getColor() const {
    return color;
}

Object::~Object() {
    reset();
}

void Object::reset() {
    if (vertices != nullptr) delete[] vertices;
    if (indices  != nullptr) delete[] indices;
    vertices = nullptr;
    indices = nullptr;
}