#include "Texture.h"

#include <fstream>
#include <sstream>

#ifdef __APPLE__
#define GL_SILENCE_DEPRECATION
#include <GL/glew.h>
#include <OpenGL/gl.h>
#include <GLUT/glut.h>
#else
#include <GL/glew.h>
#include <GL/gl.h>
#include <GL/glut.h>
#endif

Texture::Texture(): rgba(nullptr) {}

Texture::Texture(const char* filename) {
    std::ifstream txFile(filename);

    if (txFile.is_open()) {
        txFile >> width >> height >> swrap >> twrap >> f;
        unsigned int nPixels = width * height;

        rgba = new unsigned int[nPixels];
        for (int i = 0; i < nPixels; i++) {
            txFile >> rgba[i];
        }

        // set up GPU texturing
        glGenTextures(1, &txId);
        glBindTexture(GL_TEXTURE_2D, txId);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, swrap ? GL_REPEAT : GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, twrap ? GL_REPEAT : GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, rgba);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else {
        std::stringstream ss;
        ss << "Unable to open texture file " << filename;
        throw ss.str();
    }
    txFile.close();
}

void Texture::use() {
    use(0);
}

void Texture::use(unsigned int index) {
    glActiveTexture(GL_TEXTURE0 + index);
    glBindTexture(GL_TEXTURE_2D, txId);
}

void Texture::unuse() {
    glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture::free() {
    unuse();
    glDeleteTextures(1, &txId);
    if (rgba != nullptr) delete[] rgba;
    rgba = nullptr;
}

const unsigned int* Texture::getRGBA() const {
    return rgba;
}

const unsigned int Texture::getWidth() const {
    return width;
}

const unsigned int Texture::getHeight() const {
    return height;
}

const float Texture::getF() const {
    return f;
}

std::ostream& operator<<(std::ostream& o, const Texture& tx) {
    o << "Width: " << tx.width << ", " << "Height: " << tx.height << std::endl;
    for (int row = 0; row < tx.height; row++) {
        for (int col = 0; col < tx.width; col++) {
            o << tx.rgba[row*tx.width + col];
            if (col < tx.width - 1) o << ", ";
            else o << std::endl;
        }
    }
    return o;
}