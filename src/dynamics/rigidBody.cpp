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

void RigidBody::addParticle(const QVector3D &_localPos, const ParticleWeakPtr _particle)
{
//    mlog<<"passed particle";
    m_particles.push_back(_particle);


    RestShape shapeParticle;
    shapeParticle.localPos = _localPos;
    shapeParticle.particle = _particle;

    m_restShape.push_back(shapeParticle);
//    m_restShape.particle = _particle;
//    m_restShape.localPos = _particle.lock()->x;
}

ConstraintPtr RigidBody::createConstraint()
{
//    return nullptr;
//    auto smCstr = std::make_shared<ShapeMatchingConstraint>(m_particles);
    auto smCstr = std::make_shared<ShapeMatchingConstraint>(this);
    std::weak_ptr<ShapeMatchingConstraint> smCstrWeak = smCstr;
    for(auto p : m_particles)
    {
        if(auto particle = p.lock())
        {
            particle->m_Constraints.push_back(smCstrWeak);
        }
    }
    mlog<<"hello";
    return smCstr;
}

void RigidBody::setTransform(const Transform _trans)
{
    m_transfrom = _trans;
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
