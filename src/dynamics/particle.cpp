#include "dynamics/particle.h"

Particle::Particle()
{

}

//Particle::Particle(float _x, float _y, float _z, int _ID)
//{
//    Particle(_x, _y, _z, _ID, 0.5);
//}

Particle::Particle(float _x, float _y, float _z, int _ID)
{
    r = 0.5;
    m = 0.010;
    x = QVector3D(_x, _y, _z);
    v = QVector3D(0, 0, 0);
    w = 1.0/m;

    mlog<<"Particle "<<m<<" "<<w;
}

void Particle::setRadius(float _radius)
{
    r = _radius;
}

void Particle::setMass(float _mass)
{
//    if(_mass <= 0)
//        w = 0; m = 0; return;
//    m = _mass;
//    w = 1/_mass;
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

float Particle::mass()
{
    return m;
}

ParticlePtr Particle::pointer(Particle *ptr)
{
    return  shared_from_this();
}

std::vector<ParticleWeakPtr> &Particle::getParticles()
{
    m_particlesPlaceholder.clear();
    return m_particlesPlaceholder;
}

void Particle::pinToPosition(const QVector3D &_pos)
{

}

void Particle::endPinToPosition()
{
    m_Constraints.clear();
}

void Particle::pointer(Particle **_ptr)
{
    *_ptr = this;
}

const QMatrix4x4 Particle::getTransfrom()
{
    QMatrix4x4 mat;
    mat.setToIdentity();
    mat.scale(QVector3D(2*r, 2*r, 2*r));
    mat(0,3) = x.x();
    mat(1,3) = x.y();
    mat(2,3) = x.z();
    return mat;
}

const QVector3D Particle::getTranslation()
{
    return x;
}
