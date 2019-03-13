#include "include/dynamics/RigidBody.h"

RigidBody::RigidBody()
{

}

RigidBody::RigidBody(ModelPtr _model)
{
    m_model = _model;
}

void RigidBody::parseData(const ModelPtr _model)
{
//    for(unsigned int i = 0; i < _model->getNumShapes(); i++)
//    {
//        ShapePtr shape = _model->getShape(i);
//        for(auto vert : shape->getVertices())
//        {
//        }
//    }
}

void RigidBody::addParticle(const ParticleWeakPtr _particle)
{
    mlog<<"passed particle";
    m_particles.push_back(_particle);
}

void RigidBody::setTransform(const QMatrix4x4 &_mat4)
{

}

void RigidBody::pinToPosition(const QVector3D &_pos)
{

}

void RigidBody::endPinToPosition()
{

}

const QMatrix4x4 &RigidBody::getTransfrom()
{
    return m_transfrom.toMatrix();
}

const QVector3D RigidBody::getTranslation()
{

}
