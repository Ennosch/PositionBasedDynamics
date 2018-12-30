#include "dynamics/dynamicsWorld.h"

DynamicObject::DynamicObject()
{

}

DynamicObject::DynamicObject(ParticlePtr _particle)
{
    pParticle = _particle;
    m_IsParticle = true;
}

const QMatrix4x4 &DynamicObject::getTransfrom()
{
    if(m_IsParticle)
    {
        m_transform.setTranslation(pParticle->position());
        auto testMat = m_transform.toMatrix();
        return m_transform.toMatrix();
    }
    else
    {
        return m_transform.toMatrix();
    }
}
