#include "include/dynamics/collisiondetection.h"

#include <Qdebug>

CollisionDetection::CollisionDetection()
{
    qDebug()<<"CollisionDetection  Ctor";
}

float CollisionDetection::checkPointPlane(const Vec3 &_p, const Vec3 &_n, const Vec3 &_o)
{
    Vec3 temp = (_p - _o);
    float fDist (Vec3::dotProduct(temp, _n));
    return fDist;
}

Vec3 CollisionDetection::checkRayPlane(const Vec3 &_r1, const Vec3 &_r2, const Vec3 &_n, const Vec3 &_o)
{
    Vec3 ray, planehit;
    float devisor, t;
    ray = (_r2 -_r1).normalized();
    devisor = Vec3::dotProduct(ray, _n);
    t = Vec3::dotProduct((_o -_r1), _n) / devisor;
    planehit = _r1 + (t) * ray;
    return planehit;
}
