#ifndef PARTICLE_H
#define PARTICLE_H

#include <QVector3D>
#include <QDebug>

#include "utils.h"

class AbstractConstraint;

class Particle
{
public:
    Particle();
    Particle(float _x, float _y, float _z, int _ID);

    void setCell(int _i, int _j, int _k);
    void setHash(size_t _hashv);
    int ID = 0;

    QVector3D position();
    unsigned radius();

    float foo();

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
inline unsigned Particle::radius(){ return r; };

#endif // PARTICLE_H
