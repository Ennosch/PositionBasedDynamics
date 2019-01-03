#include "include/dynamics/collisiondetection.h"

#include <Qdebug>

#include <math.h>

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

bool CollisionDetection::checkRaySphere(const Vec3 &_o, const Vec3 &_d, const Vec3 &_p, float r)
{
    Vec3 L = _o - _p;
    float a = Vec3::dotProduct(_d, _d);
    float b = 2 * Vec3::dotProduct(_d,L);
    float c = Vec3::dotProduct(L,L) - pow(r,2);

    float t0, t1;
    if (!solveQuadratic(a, b, c, t0, t1)) return false;

    if (t0 > t1) std::swap(t0, t1);

            if (t0 < 0) {
                t0 = t1; // if t0 is negative, let's use t1 instead
                if (t0 < 0) return false; // both t0 and t1 are negative
            }

            float t = t0;

    qDebug()<<t;
    return true;
}

bool CollisionDetection::solveQuadratic(const float &a, const float &b, const float &c, float &x0, float &x1)
{
    float discr = b * b - 4 * a * c;
    if (discr < 0) return false;
    else if (discr == 0) x0 = x1 = - 0.5 * b / a;
    else {
        float q = (b > 0) ?
            -0.5 * (b + sqrt(discr)) :
            -0.5 * (b - sqrt(discr));
        x0 = q / a;
        x1 = c / q;
    }
    if (x0 > x1) std::swap(x0, x1);

    return true;
}
