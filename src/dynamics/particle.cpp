#include "dynamics/particle.h"

Particle::Particle()
{
        qDebug()<<"INit Particle 1 Ctor";
}

Particle::Particle(float _x, float _y, float _z, int _ID)
    :ID(_ID)
{
    qDebug()<<"INit Particle 2 Ctor";
    r = 0.5;
    m = 1.0;
    x = QVector3D(_x, _y, _z);
    v = QVector3D(0, 0, 0);
    w = 1/m;
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

float Particle::foo()
{
    return 2.0;
}

void Particle::setTransform(const QMatrix4x4 &_mat4)
{
    w = 0.0;
}

void Particle::pinToPosition(const QVector3D &_pos)
{

}

void Particle::endPinToPosition()
{
    m_Constraints.clear();
}

const QMatrix4x4 &Particle::getTransfrom()
{
    Transform trans;
    trans.setTranslation(x);
    return trans.toMatrix();
}

const QVector3D Particle::getTranslation()
{
    return x;
}
