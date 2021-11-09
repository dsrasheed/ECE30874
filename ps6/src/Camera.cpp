#include <math.h>
#include <iostream>
#include <fstream>
#include <sstream>

#include "util.h"
#include "Camera.h"
#include "Object.h"
#include "VertexArray.h"
#include "Shader.h"
#include "Scene.h"
#include "Mat3.h"
#include "Vec3.h"
#include "FrameBuffer.h"

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



Camera::Camera(float fov, float asp, float n, float f, Shader s): fov(fov), asp(asp), n(n), f(f), s(s), disp_mode(SOLID), proj_type(PERSPECTIVE), shading_type(FLAT), spec_type(PHONG) {
    lookat(Vec3(0, -40, 0), Vec3(0, 0, 0), Vec3(0, 0, 1));
    genProjectionMatrix();
}

Camera::Camera(const char* filename, float asp, Shader s): asp(asp), s(s), disp_mode(SOLID), proj_type(PERSPECTIVE), shading_type(FLAT), spec_type(PHONG) {
    std::ifstream f(filename);
    if (f.is_open()) {
        f >> fov >> n >> this->f;
        float x, y, z;
        f >> x >> y >> z;
        Vec3 eye(x, y, z);
        f >> x >> y >> z;
        Vec3 center(x, y, z);
        f >> x >> y >> z;
        Vec3 up(x, y, z);
        lookat(eye, center, up);
        genProjectionMatrix();
    }
    else {
        std::stringstream ss;
        ss << "Unable to open camera configuration file " << filename;
        throw ss.str();
    }
    f.close();
}

void Camera::lookat(const Vec3& eye, const Vec3& center, const Vec3& up) {
    cam_eye = eye;
    cam_dir = (eye - center).normalize();
    cam_right = up.cross(cam_dir).normalize();
    cam_up = cam_dir.cross(cam_right).normalize();
    genLookatMatrix();
}

// Translate, Tilt, Pan, Roll
void Camera::translate(const Vec3& d) {
    cam_eye += d;
    genLookatMatrix();
}

void Camera::tilt(const float degrees) {
    float theta = deg2rad(degrees);
    Vec3 dir = cam_dir;
    Vec3 up  = cam_up;
    cam_dir = (cos(theta) * dir - sin(theta) * up).normalize();
    cam_up  = (sin(theta) * dir + cos(theta) * up).normalize();
    genLookatMatrix();
}

void Camera::pan(const float degrees) {
    float theta = deg2rad(degrees);
    Vec3 right = cam_right;
    Vec3 dir = cam_dir;
    cam_right = (cos(theta) * right - sin(theta) * dir).normalize();
    cam_dir   = (sin(theta) * right + cos(theta) * dir).normalize();
    genLookatMatrix();
}

void Camera::roll(const float degrees) {
    float theta = deg2rad(degrees);
    Vec3 up = cam_up;
    Vec3 right = cam_right;
    cam_right = (cos(theta) * right - sin(theta) * up).normalize();
    cam_up    = (sin(theta) * right + cos(theta) * up).normalize();
    genLookatMatrix();
}

// Zoom
void Camera::zoom(float f) {
    fov = fmin(fov * f, 179.0);
    genProjectionMatrix();
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

void Camera::genOrthoProjMatrix() {
    s.use();
    float t = n * tan(deg2rad(fov/2.0));
    float r = asp * t;
    float projection[16] = {
        1 / r, 0, 0, 0,
        0, 1 / t, 0, 0,
        0, 0, -2 / (f - n), -(n + f) / (f - n),
        0, 0, 0, 1
    };
    s.setMat4("projection", projection);
    s.unuse();
}

void Camera::genPerspProjMatrix() {
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

void Camera::genProjectionMatrix() {
    if (proj_type == PERSPECTIVE) genPerspProjMatrix();
    else genOrthoProjMatrix();
}

void Camera::genModelMatrix(const Mat3& fn, const Vec3& tr) {
    s.use();
    float transform[16] = {
        fn[0][0], fn[0][1], fn[0][2], 0,
        fn[1][0], fn[1][1], fn[1][2], 0,
        fn[2][0], fn[2][1], fn[2][2], 0,
        0,        0,        0,        0
    };
    s.setMat4("transform", transform);
    s.setVec3("translate", tr[0], tr[1], tr[2]);
    s.unuse();
}

// Rendering Configuration
void Camera::setObjectDisplayMode(ObjectDisplayMode mode) {
    disp_mode = mode;
}

void Camera::setProjectionType(ProjectionType type) {
    proj_type = type;
    genProjectionMatrix();
}

void Camera::setShadingType(ShadingType type) {
    shading_type = type;
}

void Camera::setSpecType(SpecularType type) {
    spec_type = type;
}

// Rendering
void Camera::renderCPU(const Scene& scene) {
    glClearColor(1.0, 1.0, 1.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Camera::render(const Scene& scene) { 
    // Set scene uniforms
    s.use();
    s.setInt("nLights", scene.getNumLights());
    for (int i = 0; i < scene.getNumLights(); i++) {
        std::stringstream ss;
        ss << "lights[" << i << "]";
        s.setLight(ss.str(), scene.getLights()[i]);
    }
    s.setVec3("eye", cam_eye.x, cam_eye.y, cam_eye.z);

    // For each object
    int nObjs = scene.getNumObjects();
    const Object* objs = scene.getObjects();
    for (int i = 0; i < nObjs; i++) {
        render(objs[i]);
    }
}

void Camera::render(const Object& obj) {
    VertexArray v = shading_type == FLAT ? obj.getFlatVa() : obj.getSmoothVa();
    genModelMatrix(obj.getTransformMatrix(), obj.getTranslationVector());

    v.use();
    s.use();
    s.setMaterial("material", obj.getMaterial());
    s.setBool("phong", spec_type == PHONG);
    glDrawArrays(disp_mode == SOLID ? GL_TRIANGLES : GL_LINE_STRIP, 0, obj.getNumTriangles() * 3);
    s.unuse();
}

void Camera::genFrustumVertices(Object& frustum) {
  // Image Plane Vertices
  float nh = n * tan(deg2rad(fov/2));
  float nw = nh * asp;
  Vec3 nPlane = cam_eye - n*cam_dir;
  Vec3 nTop = nPlane + nh*cam_up;
  Vec3 nTopR = nTop + nw*cam_right;
  Vec3 nTopL = nTop - nw*cam_right;
  Vec3 nBot = nPlane - nh*cam_up;
  Vec3 nBotR = nBot + nw*cam_right;
  Vec3 nBotL = nBot - nw*cam_right;

  // Far Plane Vertices
  float fh, fw;
  if (proj_type == Camera::PERSPECTIVE) {
    fh = f * tan(deg2rad(fov/2));
    fw = fh * asp;
  }
  else {
    fh = nh;
    fw = nw;
  }
  Vec3 fPlane = cam_eye - f*cam_dir;
  Vec3 fTop = fPlane + fh*cam_up;
  Vec3 fTopR = fTop + fw*cam_right;
  Vec3 fTopL = fTop - fw*cam_right;
  Vec3 fBot = fPlane - fh*cam_up;
  Vec3 fBotR = fBot + fw*cam_right;
  Vec3 fBotL = fBot - fw*cam_right;

  Vec3 vectors[] = {
    nTopL, nTopR, nBotR, nBotL,
    fTopL, fTopR, fBotR, fBotL
  };
  float vertices[8*3];
  for (int i = 0; i < 8; i++) {
    vertices[i*3+0] = vectors[i][0];
    vertices[i*3+1] = vectors[i][1];
    vertices[i*3+2] = vectors[i][2];
  }
  unsigned int triangles[] = {
    // Left Plane
    3, 7, 4,
    3, 4, 0,
    // Right Plane
    1, 5, 6,
    1, 6, 2,
    // Bottom Plane
    2, 6, 7,
    2, 7, 3,
    // Top Plane
    0, 4, 5,
    0, 5, 1
  };
  frustum.setVertices(8, vertices, 8, triangles);
}

Vec3 Camera::get_eye() const {
    return cam_eye;
}

Vec3 Camera::get_dir() const {
    return cam_dir;
}

Vec3 Camera::get_up() const {
    return cam_up;
}

Vec3 Camera::get_right() const {
    return cam_right;
}

float Camera::get_n() const {
    return n;
}

float Camera::get_f() const {
    return f;
}

float Camera::get_asp() const {
    return asp;
}

float Camera::get_fov() const {
    return fov;
}

Camera::ProjectionType Camera::get_proj_type() const {
    return proj_type;
}

Camera::~Camera() {}

std::ostream& operator<<(std::ostream& o, const Camera& c) {
    o << "Cam Eye  : " << c.cam_eye.normalize()   << ", Norm: " << c.cam_eye.norm()   << std::endl
      << "Cam Dir  : " << c.cam_dir   << ", Norm: " << c.cam_dir.norm()   << std::endl
      << "Cam Up   : " << c.cam_up    << ", Norm: " << c.cam_up.norm()    << std::endl
      << "Cam Right: " << c.cam_right << ", Norm: " << c.cam_right.norm() << std::endl
      << "-----------------------" << std::endl;
    return o;
}