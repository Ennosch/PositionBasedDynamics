#ifndef PARTICLE_H
#define PARTICLE_H

#include <QVector3D>
#include <QDebug>

#include "dynamics/dynamicUtils.h"
#include "dynamics/dynamicObject.h"

class AbstractConstraint;

//class Particle : public DynamicObject, public std::enable_shared_from_this<Particle>
class Particle : public std::enable_shared_from_this<Particle>
{
public:
    Particle();
    Particle(float _x, float _y, float _z, int _ID);
//    Particle(float _x, float _y, float _z, int _ID, float _r);

    void setRadius(float _radius);
    void setMass(float _mass);
    void setCell(int _i, int _j, int _k);
    void setHash(size_t _hashv);
    int ID = 0;
    int bodyID = 0;

    QVector3D position();
    float radius();
    float mass();

    ParticlePtr pointer(Particle *ptr);
    std::vector<ParticleWeakPtr>& getParticles();

    // virtuals
    void pinToPosition(const QVector3D &_pos);
    void endPinToPosition();

    void pointer(Particle** _ptr);
    const QMatrix4x4 getTransfrom();
    const QVector3D getTranslation();



//private:
    unsigned age;
    float w, r, m;
    float collisionGradLen = 0;
    size_t hash;
    QVector3D p, pp, v, vt, x, xt, qc;
    QVector3D collisionVector;
    int3 cell;

    std::vector<ParticleWeakPtr> m_particlesPlaceholder;

    std::vector<ConstraintPtr> m_CollisionConstraints;
    std::vector<ConstraintPtr> m_CollisionConstraints_B;
    std::vector<ConstraintPtr> m_PreConditionConstraints;
//    std::list<ParticleWeakPtr> m_NonCollisionParticles;
    std::list<ParticlePtr> m_NonCollisionParticles;

    std::vector<ConstraintWeakPtr> m_Constraints;

};

inline QVector3D Particle::position(){ return x; };
inline float Particle::radius(){ return r; };

#endif // PARTICLE_H
