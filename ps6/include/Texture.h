#ifndef __ECE30874_TEXTURE_H__
#define __ECE30874_TEXTURE_H__

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

class Texture {
    private:
        unsigned int width, height;
        bool swrap, twrap;
        float f;
        unsigned int* rgba;
        unsigned int txId;
    public:

    Texture(): rgba(nullptr) {}

    Texture(const char* filename) {
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
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST_MIPMAP_NEAREST);
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

    void use() {
        use(0);
    }

    void use(unsigned int index) {
        glActiveTexture(GL_TEXTURE0 + index);
        glBindTexture(GL_TEXTURE_2D, txId);
    }

    void unuse() {
        glBindTexture(GL_TEXTURE_2D, 0);
    }

    ~Texture() {
        if (rgba != nullptr) delete[] rgba;
        rgba = nullptr;
    }
};

#endif