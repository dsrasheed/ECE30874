#ifndef __ECE30834_CAMERA_H__
#define __ECE30834_CAMERA_H__

#include <iostream>

#include "Shader.h"
#include "Vec3.h"
#include "Mat3.h"
#include "Scene.h"
#include "FrameBuffer.h"

class Camera {
    public:
        enum ObjectDisplayMode { WIREFRAME, SOLID };
        enum ProjectionType { PERSPECTIVE, ORTHO };
        enum ShadingType { FLAT, SMOOTH };
        enum SpecularType { PHONG, BLINN };
    private:
        float fov;
        float asp;
        float n;
        float f;
        FrameBuffer fb;
        Shader s;
        Vec3 cam_eye;
        Vec3 cam_dir;
        Vec3 cam_right;
        Vec3 cam_up;
        ObjectDisplayMode disp_mode;
        ProjectionType proj_type;
        SpecularType spec_type;
        ShadingType shading_type;
        
        void genLookatMatrix();
        void genProjectionMatrix();
        void genOrthoProjMatrix();
        void genPerspProjMatrix();
        void genModelMatrix(const Mat3& fn, const Vec3& tr);
        void setModelColor(const Vec3& color);

    public:
        Camera(float fov, float asp, float n, float f, Shader s);
        Camera(const char* filename, float asp, Shader s);
        void lookat(const Vec3& eye, const Vec3& center, const Vec3& up);
        void translate(const Vec3& d);
        void tilt(const float degrees);
        void pan(const float degrees);
        void roll(const float degrees);
        void zoom(const float f);
        void render(const Scene& s);
        void render(const Object& obj);
        void renderCPU(const Scene& s);
        void genFrustumVertices(Object& frustum);
        void setObjectDisplayMode(ObjectDisplayMode mode);
        void setProjectionType(ProjectionType type);
        void setShadingType(ShadingType type);
        void setSpecType(SpecularType type);
        void setFrameBuffer(int w, int h);
        const FrameBuffer& getFrameBuffer() const;
        Vec3 get_eye() const;
        Vec3 get_dir() const;
        Vec3 get_up() const;
        Vec3 get_right() const;
        float get_n() const;
        float get_f() const;
        float get_asp() const;
        float get_fov() const;
        ProjectionType get_proj_type() const;
        ~Camera();

        friend std::ostream& operator<<(std::ostream& o, const Camera& c);
};

#endif