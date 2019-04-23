#ifndef PARTICLE_H
#define PARTICLE_H

#include <QVector3D>
#include <QDebug>

#include "utils.h"
#include "transform.h"
#include "dynamics/dynamicObject.h"


class AbstractConstraint;

class Particle : public DynamicObject, public std::enable_shared_from_this<Particle>
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

    ParticlePtr pointer(Particle *ptr);

    // virtuals
    void setTransform(const QMatrix4x4& _mat4);
    void pinToPosition(const QVector3D &_pos);
    void endPinToPosition();

    void pointer(Particle** _ptr);
    const QMatrix4x4& getTransfrom();
    const QVector3D getTranslation();

//private:
    unsigned age;
    float w, r, m;
    size_t hash;
    QVector3D p, pp, v, vt, x, xt, qc;
    int3 cell;

    std::vector<ConstraintPtr> m_CollisionConstraints;
    std::list<ParticleWeakPtr> m_NonCollisionParticles;

};

inline QVector3D Particle::position(){ return x; };
inline float Particle::radius(){ return r; };

#endif // PARTICLE_H
