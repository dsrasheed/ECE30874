#ifndef __ECE30834_CAMERA_H__
#define __ECE30834_CAMERA_H__

#include "Shader.h"
#include "Vec3.h"
#include "Mat3.h"
#include "Scene.h"

class Camera {
    private:
        float fov;
        float asp;
        float n;
        float f;
        Shader s;
        Vec3 cam_eye;
        Vec3 cam_dir;
        Vec3 cam_right;
        Vec3 cam_up;
        
        void genLookatMatrix();
        void genProjectionMatrix();
        void genModelMatrix(const Mat3& fn, const Vec3& tr);
        void setModelColor(const Vec3& color);
    public:
        Camera(float fov, float asp, float n, float f, Shader s);
        void lookat(const Vec3& eye, const Vec3& center, const Vec3& up);
        void translate(const Vec3& d);
        void tilt(const float degrees);
        void pan(const float degrees);
        void roll(const float degrees);
        void zoom(const float f);
        void render(const Scene& s);
        ~Camera();
};

#endif