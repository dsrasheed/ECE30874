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
#include "Mat4.h"
#include "Vec4.h"
#include "FrameBuffer.h"
#include "rgba.h"

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

inline void boundingBox(Vec3* v, unsigned int n, double* bb)
{
    bb[0] = bb[2] = bb[4] = 1e10;
    bb[1] = bb[3] = bb[5] = -1e10;
    for (unsigned int i = 0u; i < n; ++i) {
        Vec3& w = v[i];
        if (w.x < bb[0])
            bb[0] = w.x;
        if (w.x > bb[1])
            bb[1] = w.x;
        if (w.y < bb[2])
            bb[2] = w.y;
        if (w.y > bb[3])
            bb[3] = w.y;
        if (w.z < bb[4])
            bb[4] = w.z;
        if (w.z > bb[5])
            bb[5] = w.z;
    }
}

inline bool leftTurnXY(const Vec3& a, const Vec3& b, const Vec3& c)
{
    float ux = c.x - b.x, uy = c.y - b.y, vx = a.x - b.x, vy = a.y - b.y;
    return ux * vy - uy * vx >= 0.0f;
}

inline bool inTriangleXY(const Vec3& p, const Vec3& a, const Vec3& b, const Vec3& c,
    bool ccw)
{
    return leftTurnXY(a, b, p) == ccw && leftTurnXY(b, c, p) == ccw &&
        leftTurnXY(c, a, p) == ccw;
}

inline void barycentricCoordinates(const Vec3& a, const Vec3& b, const Vec3& c,
    const Vec3& p, float& k1, float& k2, float& k3)
{
    float d = (b.y - c.y) * (a.x - c.x) + (c.x - b.x) * (a.y - c.y);
    k1 = ((b.y - c.y) * (p.x - c.x) + (c.x - b.x) * (p.y - c.y)) / d;
    k2 = ((c.y - a.y) * (p.x - c.x) + (a.x - c.x) * (p.y - c.y)) / d;
    k3 = 1.0f - k1 - k2;
}

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
// LEARN: Why is it translate then rotate?
// Matrix multiplication is not commutative
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

bool Camera::frontFace(Vec3* tri) {
    Vec3 u = cam_eye - tri[1];
    Vec3 a = tri[2] - tri[1];
    Vec3 b = tri[0] - tri[1];
    Vec3 n = a.cross(b);
    return u.dot(n) > 0.0;
}

// Rendering
void Camera::renderCPU(const Scene& scene) {
    if (fb == nullptr) {
        throw "Call setFrameBuffer before calling renderCPU";
    }

    int nPixels = fb->w * fb->h;
    
    // Clear
    for (int i = 0; i < nPixels; i++) {
        fb->color[i] = RGBA(1, 1, 1).pack();
    }
    for (int i = 0; i < nPixels; i++) {
        fb->depth[i] = 1.0;
    }

    int nObjs = scene.getNumObjects();
    for (int i = 0; i < nObjs; i++) {
        const Object* obj = &scene.getObjects()[i];
        Mat3 m = obj->getTransformMatrix();
        Vec3 tr = obj->getTranslationVector();
        
        const float* data = shading_type == SMOOTH ? obj->getSmoothVerts() : obj->getFlatVerts();
        for (int j = 0; j < obj->getNumTriangles(); j++) {
            Vec3 v[] = {
                Vec3(data[j*24 + 0 + 0], data[j*24 + 0 + 1], data[j*24 + 0 + 2]),
                Vec3(data[j*24 + 8 + 0], data[j*24 + 8 + 1], data[j*24 + 8 + 2]),
                Vec3(data[j*24 + 16 + 0], data[j*24 + 16 + 1], data[j*24 + 16 + 2])
            };
            Vec3 n[] = {
                Vec3(data[j*24 + 0 + 3], data[j*24 + 0 + 4], data[j*24 + 0 + 5]),
                Vec3(data[j*24 + 8 + 3], data[j*24 + 8 + 4], data[j*24 + 8 + 5]),
                Vec3(data[j*24 + 16 + 3], data[j*24 + 16 + 4], data[j*24 + 16 + 5])
            };
            Vec3 tx[] = {
                Vec3(data[j*24 + 0 + 6], data[j*24 + 0 + 7], 0),
                Vec3(data[j*24 + 8 + 6], data[j*24 + 8 + 7], 0),
                Vec3(data[j*24 + 16 + 6], data[j*24 + 16 + 7], 0)
            };
            for (int i = 0; i < 3; i++) {
                v[i] = m * v[i] + tr;
                n[i] = m * n[i];
            }
            if (frontFace(v)) {
                Vec3 vp[3], ps[9];
                unsigned int np = clip(v, n, vp, ps);
                if (np > 0) {
                    drawFilled(v, n, vp, np, ps, scene.getNumLights(), scene.getLights(), obj->getMaterial());
                }
            }
        }
    }
}

unsigned int Camera::clip(Vec3* vs, Vec3* ns, Vec3* vp, Vec3* ps) {
    Mat4 rotate = Mat4(
        cam_right[0], cam_right[1], cam_right[2], 0.0,
        cam_up   [0], cam_up   [1], cam_up   [2], 0.0,
        cam_dir  [0], cam_dir  [1], cam_dir  [2], 0.0,
        0,            0,            0,            1.0
    );
    Mat4 translate = Mat4(
        1, 0, 0, -cam_eye[0],
        0, 1, 0, -cam_eye[1],
        0, 0, 1, -cam_eye[2],
        0, 0, 0, 1
    );
    Mat4 view = rotate * translate;
    float t = n * tan(deg2rad(fov/2.0));
    float r = asp * t;
    Mat4 proj = Mat4(
        n / r, 0, 0, 0,
        0, n / t, 0, 0,
        0, 0, (n + f) / (n - f), 2 * f * n / (n - f),
        0, 0, -1, 0
    );
    Mat4 vpm = proj * view;
    static Vec4 clipPlanes[6] = { {1.0f, 0.0f, 0.0f, -1.0f}, {-1.0f, 0.0f, 0.0f, -1.0f},
                     {0.0f, 1.0f, 0.0f, -1.0f}, {0.0f, -1.0f, 0.0f, -1.0f},
                     {0.0f, 0.0f, 1.0f, -1.0f}, {0.0f, 0.0f, -1.0f, -1.0f} };
    Vec4 w[9];
    for (unsigned int i = 0u; i < 3u; ++i) {
        w[i] = vpm * Vec4(vs[i], 1.0f);
        vp[i] = w[i].dehomogenize();
    }
    double bb[6];
    boundingBox(vp, 3, bb);
    if (bb[0] > 1.0f || bb[1] < -1.0f || bb[2] > 1.0f || bb[3] < -1.0f ||
        bb[4] > 1.0f || bb[5] < -1.0f)
        return 0u;
    unsigned int k = 3u;
    if (bb[0] < -1.0f || bb[1] > 1.0f || bb[2] < -1.0f || bb[3] > 1.0f ||
        bb[4] < -1.0f || bb[5] > 1.0f) {
        bool in[9];
        Vec4 nw[9];
        for (unsigned int i = 0u; i < 6u; ++i) {
            bool allin = true;
            for (unsigned int j = 0u; j < k; ++j) {
                in[j] = clipPlanes[i].dot(w[j]) <= 0.0f;
                allin = allin && in[j];
            }
            if (!allin) {
                unsigned int nk = 0u;
                for (unsigned int j = 0u; j < k; ++j) {
                    if (in[j])
                        nw[nk++] = w[j];
                    if (in[j] != in[(j + 1) % k]) {
                        Vec4 u(w[(j + 1) % k] - w[j]);
                        nw[nk++] = w[j] - u * (clipPlanes[i].dot(w[j]) / clipPlanes[i].dot(u));
                    }
                }
                for (unsigned int j = 0u; j < nk; ++j)
                    w[j] = nw[j];
                k = nk;
            }
        }
    }
    for (unsigned int i = 0u; i < k; ++i)
        ps[i] = w[i].dehomogenize();
    return k;
}

void Camera::drawFilled(Vec3* vs, Vec3* ns, Vec3* vp, unsigned int np, Vec3* ps, int nl,
    Light* ls, const Material& m) {
    bool ccw = leftTurnXY(ps[0], ps[1], ps[2]);
    Vec3 a = ps[0];
    for (unsigned int k = 1u; k + 1u < np; ++k) {
        Vec3 b = ps[k], c = ps[k + 1];
        unsigned int bb[4];
        fb->pixelBox(a, b, c, bb);
        for (unsigned int i = bb[0]; i <= bb[1]; ++i)
            for (unsigned int j = bb[2]; j <= bb[3]; ++j) {
                Vec3 pc = fb->pixelCenter(i, j);
                if (inTriangleXY(pc, a, b, c, ccw)) {
                    drawPixel(vs, ns, vp, nl, ls, m, pc, i, j);
                }
            }
    }
}

void Camera::drawPixel(Vec3* vs, Vec3* ns, Vec3* vp, int nl, Light* ls, const Material& m,
    const Vec3& pc, unsigned int i, unsigned int j) {
    float k1, k2, k3;
    barycentricCoordinates(vp[0], vp[1], vp[2], pc, k1, k2, k3);
    float dij = k1 * vp[0].z + k2 * vp[1].z + k3 * vp[2].z;
    if (fb->setDepth(i, j, dij)) {
        Vec3 vij = hyperbolic(vs, vp, k1, k2, k3);
        Vec3 nij = hyperbolic(ns, vp, k1, k2, k3);
        unsigned int cij = lighting(vij, nij, nl, ls, m).pack();
        fb->color[i + j * fb->w] = cij;
    }
}

Vec3 Camera::hyperbolic(Vec3* v, Vec3* vp, float k1, float k2, float k3) const {
    if (proj_type == ORTHO)
        return v[0] * k1 + v[1] * k2 + v[2] * k3;
    float z1 = cameraZ(vp[0].z), z2 = cameraZ(vp[1].z), z3 = cameraZ(vp[2].z),
        l1 = k1 / z1, l2 = k2 / z2, l3 = k3 / z3, d = l1 + l2 + l3,
        m1 = l1 / d, m2 = l2 / d, m3 = l3 / d;
    return v[0] * m1 + v[1] * m2 + v[2]* m3;
}

float Camera::cameraZ(float d) const {
        return 2.0f * f * n / ((f - n) * d - f - n);
}

RGBA Camera::lighting(const Vec3& ve, const Vec3& no, int nl, Light* ls, const Material& m) const {
    Vec3 n = no.normalize(), v = (cam_eye - ve).normalize(), c(0.0f, 0.0f, 0.0f);
    for (int i = 0u; i < nl; ++i) {
        const Light& li = ls[i];
        c = c + li.a * m.d;
        Vec3 ll = li.dir ? li.p : li.p - ve;
        if (ll.dot(n) > 0.0f) {
            float d = ll.norm(),
                k = li.dir ? 1.0f : 1.0f / (1.0f + li.kl * d + li.kq * d * d);
            Vec3 l = ll / d, h = (l + v).normalize();
            c = c + li.d * m.d * n.dot(l) * k;
            float nh = n.dot(h);
            c = c + li.s * m.s * pow(nh, m.shininess) * k;
        }
    }
    return RGBA(c.x, c.y, c.z).pack();
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
    s.unuse();

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
    if (obj.hasTx()) {
        Texture tx = obj.getTexture();
        tx.use(0);
        s.setBool("hasTx", 1);
        s.setInt("texture1", 0);
        s.setFloat("txWeight", tx.getF());
    }
    else {
        s.setBool("hasTx", 0);
    }
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

void Camera::setFrameBuffer(int w, int h) {
    fb = new FrameBuffer(w, h);
}

const FrameBuffer* Camera::getFrameBuffer() const {
    return fb;
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

Camera::~Camera() {
    if (fb != nullptr) delete fb;
    fb = nullptr;
}

std::ostream& operator<<(std::ostream& o, const Camera& c) {
    o << "Cam Eye  : " << c.cam_eye.normalize()   << ", Norm: " << c.cam_eye.norm()   << std::endl
      << "Cam Dir  : " << c.cam_dir   << ", Norm: " << c.cam_dir.norm()   << std::endl
      << "Cam Up   : " << c.cam_up    << ", Norm: " << c.cam_up.norm()    << std::endl
      << "Cam Right: " << c.cam_right << ", Norm: " << c.cam_right.norm() << std::endl
      << "-----------------------" << std::endl;
    return o;
}