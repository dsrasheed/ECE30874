#ifndef __ECE30874_FRAMEBUFFER_H__
#define __ECE30874_FRAMEBUFFER_H__

#include <iostream>

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

#include "Vec3.h"

using namespace std;

class FrameBuffer {
    public:
        int w, h;
        uint32_t* color;
        float* depth;

        FrameBuffer(): w(-1), h(-1) {
            color = nullptr;
            depth = nullptr;
        }

        FrameBuffer(int w, int h): w(w), h(h) {
            color = new uint32_t[w * h]();
            depth = new float[w * h]();
        }

        bool setDepth(unsigned int i, unsigned int j, float dij) {
            unsigned int k = i + j * w;
            float de = 0.5f * dij + 0.5f, dk = depth[k];
            if (dk <= de)
                return false;
            depth[k] = de;
            return true;
        }

        void pixelBox(const Vec3& a, const Vec3& b, const Vec3& c,
        unsigned int* bb) const {
            float umin = min(a.x, min(b.x, c.x)), umax = max(a.x, max(b.x, c.x)),
                vmin = min(a.y, min(b.y, c.y)), vmax = max(a.y, max(b.y, c.y));
            bb[0] = max(0, int(0.5f * w * (umin + 1.0f)));
            bb[1] = min(int(w) - 1, int(0.5f * w * (umax + 1.0f)));
            bb[2] = max(0, int(0.5f * h * (vmin + 1.0f)));
            bb[3] = min(int(h) - 1, int(0.5f * h * (vmax + 1.0f)));
        }

        Vec3 pixelCenter(unsigned int i, unsigned int j) const {
            return Vec3((2.0f * i + 1.0f) / float(w) - 1.0f,
                (2.0f * j + 1.0f) / float(h) - 1.0f,
                0.0f);
        }

        ~FrameBuffer() {
            if (color != nullptr) delete[] color;
            if (depth != nullptr) delete[] depth;
            color = nullptr;
            depth = nullptr;
        }
};

#endif