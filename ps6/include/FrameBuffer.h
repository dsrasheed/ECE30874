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
        uint8_t* color;
        float* depth;

        FrameBuffer(int w, int h) {
            color = new uint8_t[w * h * 4]();
            depth = new float[w * h]();
        }

        ~FrameBuffer()  {
            delete[] color;
            delete[] depth;
        }
};

#endif