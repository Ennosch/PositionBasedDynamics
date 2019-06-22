#include "include/dynamics/singleParticle.h"
#include "include/dynamics/particle.h"

SingleParticle::SingleParticle(ParticlePtr _p) : m_particle(_p)
{
    m_radius = _p->radius();
}

const QMatrix4x4 SingleParticle::getTransfrom()
{
    QMatrix4x4 t;
    t.setToIdentity();
//    t.scale(2 * m_radius * QVector3D(1,1,1));
//    t.scale(QVector3D(2,2,2));
//    mlog<<" ----------------------- r: "<<m_radius;

    t.translate(m_particle->x);
    t.scale((2 * m_radius) * QVector3D(1,1,1));
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

std::vector<ParticleWeakPtr> &SingleParticle::getParticles()
{
    std::vector<ParticleWeakPtr> vec;
    vec.empty();
    return vec;
}

int SingleParticle::numParticles()
{
    return 0;
}
