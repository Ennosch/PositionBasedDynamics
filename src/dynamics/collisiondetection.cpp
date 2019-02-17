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

float CollisionDetection::checkRaySphereF(const Vec3 &_o, const Vec3 &_d, const Vec3 &_p, float r)
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
                if (t0 < 0) return 0.0; // both t0 and t1 are negative
            }

            float t = t0;

//    qDebug()<<t;
    return t;
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

Vec3 CollisionDetection::intersectRayPlane(const Vec3 &_n, const Vec3 &_pO, const Ray &_ray)
{
//    mlog<<_n<<_pO;
    double t;
    double denom = Vec3::dotProduct(_n, _ray.Dir);
    if (denom > 1e-12)
    {
//        qDebug()<<"WARNING intersectRayPlane parallel"<<denom;
    }
        Vec3 pOlO = _pO - _ray.Origin;
        t = Vec3::dotProduct(pOlO, _n) / denom;
        QVector3D v1 = _ray.Origin + (_ray.Dir.normalized() * t);
        QVector3D resultEnd = v1 + _ray.Dir.normalized();
        QVector3D precise = intersectLinePlane(_n, _pO, v1, resultEnd);
        return precise;
}

Vec3 CollisionDetection::intersectLinePlane(const Vec3 &_n, const Vec3 &_pO, const Vec3 &_A, const Vec3 &_B)
{
    Vec3 AB = _B -_A;
    double t = (_pO.length() - Vec3::dotProduct(_n, _A)) / Vec3::dotProduct(_n, AB);
    return _A + (t * AB);
}

Vec3 CollisionDetection::closetPointFromRayToRay(const Ray &_r1, const Ray &_r2)
{

    float p,q,r,s,t,d,e;
    Vec3 a,b,c;
    a = _r1.Dir.normalized();
    b = _r2.Dir.normalized();
    c = _r2.Origin - _r1.Origin;

    p = Vec3::dotProduct(a, b);
    q = Vec3::dotProduct(a, c);
    r = Vec3::dotProduct(b, c);
    s = Vec3::dotProduct(a, a);
    t = Vec3::dotProduct(b, b);

    d = (-p*r + q*t) / (s*t - p*p);
    e = (p*q) / (s*t - p*p) - (r / ( t - ((p*p) / s)));

    Vec3 point1 =  _r1.Origin +  (a * d);
    Vec3 point2 =  _r2.Origin + (b * e);

    Vec3 z = point2 - point1;

    Vec3 result = point1 + z;

//    mlog<<_r2.Origin<<_r2.Dir;
//    mlog<<"cd:"<<_r1.Origin<<_r1.Dir;


    return result;
}































