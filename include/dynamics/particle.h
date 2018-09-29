#ifndef PARTICLE_H
#define PARTICLE_H

#include <QVector3D>
#include <QDebug>

#include "utils.h"




class Particle
{
public:
    Particle();
    Particle(float _x, float _y, float _z);

    void setCell(int _i, int _j, int _k);
    void setHash(size_t _hashv);
    int ID = 5;

    QVector3D position();


private:
    unsigned m, r, age;
    size_t hash;
    QVector3D v, vt, x, xt;
    int3 cell;

};

inline QVector3D Particle::position(){ return x; };

#endif // PARTICLE_H
