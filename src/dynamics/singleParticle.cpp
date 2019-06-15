#include "include/dynamics/singleParticle.h"
#include "include/dynamics/particle.h"

SingleParticle::SingleParticle(ParticlePtr _p) : m_particle(_p)
{

}

const QMatrix4x4 SingleParticle::getTransfrom()
{
    QMatrix4x4 t;
    t.setToIdentity();
    t.translate(m_particle->x);
    return  t;
}

const QVector3D SingleParticle::getTranslation()
{
    return m_particle->x;
}

void SingleParticle::pinToPosition(const QVector3D &_pos)
{

}

void SingleParticle::endPinToPosition()
{

}

ParticlePtr SingleParticle::pointer(Particle *ptr)
{
    return m_particle;
}
