#ifndef COLLISIONDETECTION_H
#define COLLISIONDETECTION_H

#include <vector>
#include <QMatrix4x4>
#include <QVector3D>

#include "utils.h"

typedef QVector3D Vec3;

class CollisionDetection
{
public:
    CollisionDetection();
    float checkPointPlane(const Vec3 &_p, const Vec3 &_n, const Vec3 &_o);
    Vec3 checkRayPlane(const Vec3 &_r1, const Vec3 &_r2, const Vec3 &_n, const Vec3 &_o);

    bool checkRaySphere(const Vec3 &_o, const Vec3 &_d, const Vec3 &_p, float r);
    float checkRaySphereF(const Vec3 &_o, const Vec3 &_d, const Vec3 &_p, float r);
    bool solveQuadratic(const float &a, const float &b, const float &c, float &x0, float &x1);

    Vec3 intersectRayPlane(const Vec3 &_n, const Vec3 &_pO, const Ray &_ray);
    Vec3 intersectLinePlane(const Vec3 &_n, const Vec3 &_pO, const Vec3 &_A, const Vec3 &_B);

    Vec3 closetPointFromRayToRay(const Ray &_r1, const Ray &_r2);


    Scene *m_scene;
};


#endif // COLLISIONDETECTION_H
