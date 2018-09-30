#include "dynamics/particle.h"

Particle::Particle()
{

}

Particle::Particle(float _x, float _y, float _z, int _ID)
    :ID(_ID)
{
    x = QVector3D(_x, _y, _z);
}

void Particle::setCell(int _i, int _j, int _k)
{
    cell.i = _i;
    cell.j = _j;
    cell.k = _k;
}

void Particle::setHash(size_t _hashv)
{
    hash = _hashv;
}


