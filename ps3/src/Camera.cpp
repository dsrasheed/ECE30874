#include <math.h>

#include "util.h"
#include "Camera.h"
#include "Object.h"
#include "VertexArray.h"
#include "Shader.h"
#include "Scene.h"
#include "Mat3.h"
#include "Vec3.h"

#ifdef __APPLE__
#define GL_SILENCE_DEPRECATION
#include <GL/glew.h>
#include <OpenGL/gl.h>
#include <GLUT/glut.h>
#else
// TODO: Ask what headers should be for them
#include <GL/glew.h>
#include <GL/gl.h>
#include <GL/glut.h>
#endif

Camera::Camera(float fov, float asp, float n, float f, Shader s): fov(fov), asp(asp), n(n), f(f), s(s) {
    lookat(Vec3(0, -40, 0), Vec3(0, 0, 0), Vec3(0, 0, 1));
}

void Camera::lookat(const Vec3& eye, const Vec3& center, const Vec3& up) {
    cam_eye = eye;
    cam_dir = (eye - center).normalize();
    cam_right = up.cross(cam_dir).normalize();
    cam_up = cam_dir.cross(cam_right).normalize();
}

// Translate, Tilt, Pan, Roll
void Camera::translate(const Vec3& d) {
    cam_eye += d;
}

void Camera::tilt(const float degrees) {
    float theta = deg2rad(degrees);
    Vec3 dir = cam_dir;
    Vec3 up  = cam_up;
    cam_dir = cos(theta) * dir - sin(theta) * up;
    cam_up  = sin(theta) * dir + cos(theta) * up;
}

void Camera::pan(const float degrees) {
    float theta = deg2rad(degrees);
    Vec3 right = cam_right;
    Vec3 dir = cam_dir;
    cam_right = cos(theta) * right - sin(theta) * dir;
    cam_dir   = sin(theta) * right + cos(theta) * dir;
}

void Camera::roll(const float degrees) {
    float theta = deg2rad(degrees);
    Vec3 up = cam_up;
    Vec3 right = cam_right;
    cam_right = cos(theta) * right - sin(theta) * up;
    cam_up    = sin(theta) * right + cos(theta) * up;
}

// Zoom
void Camera::zoom(float f) {
    fov *= f;
}

// Render helpers
void Camera::genLookatMatrix() {
    s.use();
    float rotateScene[16] = {
        cam_right[0], cam_right[1], cam_right[2], 0.0,
        cam_up   [0], cam_up   [1], cam_up   [2], 0.0,
        cam_dir  [0], cam_dir  [1], cam_dir  [2], 0.0,
        0,            0,            0,            1.0
    };
    s.setMat4("rotateScene", rotateScene);
    float translateScene[16] = {
        1, 0, 0, -cam_eye[0],
        0, 1, 0, -cam_eye[1],
        0, 0, 1, -cam_eye[2],
        0, 0, 0, 1
    };
    s.setMat4("translateScene", translateScene);
    s.unuse();
}

void Camera::genProjectionMatrix() {
    s.use();
    float t = n * tan(deg2rad(fov/2.0));
    float r = asp * t;
    float projection[16] = {
        n / r, 0, 0, 0,
        0, n / t, 0, 0,
        0, 0, (n + f) / (n - f), 2 * f * n / (n - f),
        0, 0, -1, 0
    };
    s.setMat4("projection", projection);
    s.unuse();
}

void Camera::genModelMatrix(const Mat3& fn, const Vec3& tr) {
    s.use();
    float model[16] = {
        fn[0][0], fn[0][1], fn[0][2], tr[0],
        fn[1][0], fn[1][1], fn[1][2], tr[1],
        fn[2][0], fn[2][1], fn[2][2], tr[2],
        0,        0,        0,        1.0
    };
    s.setMat4("model", model);
    s.unuse();
}

void Camera::setModelColor(const Vec3& color) {
    s.use();
    s.setVec3("color", color[0], color[1], color[2]);
    s.unuse();
}

// Rendering
void Camera::render(const Scene& scene) { 
    genProjectionMatrix();
    genLookatMatrix();

    // For each object
    int nObjs = scene.getNumObjects();
    const Object* objs = scene.getObjects();
    for (int i = 0; i < nObjs; i++) {
        VertexArrayIndex v(
            objs[i].getNumVertices(), objs[i].getVertices(),
            objs[i].getNumTriangles()*3, objs[i].getTriangles()
        );
        //std::cout << "Num Vertices  : " << objs[i].getNumVertices() << std::endl;
        //std::cout << "Num Triangles : " << objs[i].getNumTriangles() << std::endl;
        genModelMatrix(objs[i].getTransformMatrix(), objs[i].getTranslationVector());
        setModelColor(objs[i].getColor());

        v.use();
        s.use();
        glDrawArrays(GL_TRIANGLES, 0, objs[i].getNumTriangles() * 3);
        v.free();
        s.unuse();
    }
}

Camera::~Camera() {
}