#ifndef COLLISIONDETECTION_H
#define COLLISIONDETECTION_H

#include <vector>
#include <QMatrix4x4>
#include <QVector3D>


typedef QVector3D Vec3;

class CollisionDetection
{
public:
    CollisionDetection();
    float checkPointPlane(const Vec3 &_p, const Vec3 &_n, const Vec3 &_o);
    Vec3 checkRayPlane(const Vec3 &_r1, const Vec3 &_r2, const Vec3 &_n, const Vec3 &_o);
};


#endif // COLLISIONDETECTION_H
