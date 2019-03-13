#ifndef PARTICLE_H
#define PARTICLE_H

#include <QVector3D>
#include <QDebug>

#include "utils.h"
#include "transform.h"
#include "dynamics/dynamicObject.h"


class AbstractConstraint;

class Particle : public DynamicObject
{
public:
    Particle();
    Particle(float _x, float _y, float _z, int _ID);

    void setCell(int _i, int _j, int _k);
    void setHash(size_t _hashv);
    int ID = 0;

    QVector3D position();
    float radius();
    float foo();

    // virtuals
    void setTransform(const QMatrix4x4& _mat4);
    void pinToPosition(const QVector3D &_pos);
    void endPinToPosition();

    const QMatrix4x4& getTransfrom();
    const QVector3D getTranslation();

//private:
    unsigned age;
    float w, r, m;
    size_t hash;
    QVector3D p, pp, v, vt, x, xt, qc;
    int3 cell;
//    std::vector<ConstraintWeakPtr> m_CollisionConstraints;
    std::vector<ConstraintPtr> m_CollisionConstraints;
    std::vector<ConstraintWeakPtr> m_Constraints;

};

inline QVector3D Particle::position(){ return x; };
inline float Particle::radius(){ return r; };

#endif // PARTICLE_H
