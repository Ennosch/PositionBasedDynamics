#include "dynamics/dynamicsWorld.h"

DynamicObject::DynamicObject()
{

}


void DynamicObject::setTransform(const QMatrix4x4 &_mat4)
{
//    if(m_IsParticle)
//    {
//        pParticle->w = 0;
//    }
}

void DynamicObject::pinToPosition(const QVector3D &_pos)
{

    mlog<<"DynamicObject::pinToPosition";
//    if(m_IsParticle)
//    {
//        auto pinCnstr = std::make_shared<PinConstraint>(pParticle, _pos);
//        pParticle->m_Constraints.push_back(pinCnstr);
//    }
}

void DynamicObject::endPinToPosition()
{
//    if(m_IsParticle)
//    {
//        pParticle->m_Constraints.clear();
    //    }
}

//DynamicObject *DynamicObject::pointer()
//{
//    mlog<<"DynamicObject::pointer";
//    return nullptr;
//}

const QMatrix4x4 DynamicObject::getTransfrom()
{
//    if(m_IsParticle)
//    {
//        m_transform.setTranslation(pParticle->position());
//        return m_transform.toMatrix();
//    }
//    else
//    {
//        return m_transform.toMatrix();
//    }
}

const QVector3D DynamicObject::getTranslation()
{
//    if(m_IsParticle)
//    {
//        return pParticle->position();
//    }
}
