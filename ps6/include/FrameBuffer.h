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
};

#endif