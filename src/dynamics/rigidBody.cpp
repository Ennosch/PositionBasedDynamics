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
    m_restShape.push_back(_localPos);

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
            for_each(m_particles.begin(), m_particles.end(), [&particle](ParticleWeakPtr pNeighbour)
            {
                if(particle != pNeighbour.lock())
                    particle->m_NonCollisionParticles.push_back(pNeighbour);
            });
        }
    }
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

void RigidBody::updateModelBuffers()
{
    for(unsigned int i = 0; i < m_model->getNumShapes(); i++)
    {
        ShapePtr shape = m_model->getShape(i);
//        for(auto pair : shape->getVertsMap())
        for(int i=0; i < shape->getVertsMap().size();  i++)
        {
            QVector3D position = m_particles[i].lock()->getTranslation();
            for(auto vertIdx : shape->getVertsMap()[i])
            {
                shape->setVertexPositionAtIndex(vertIdx, position);
            }
//            mlog<<" pointIdx: "<<pointIdx<<"    list: "<<pair.second;
        }
        shape->updateVertexBuffer();
    }
}

const QMatrix4x4 &RigidBody::getTransfrom()
{
    updateModelBuffers();
    return m_transfrom.toMatrix();
}

const QVector3D RigidBody::getTranslation()
{

}

//std::shared_ptr<RigidBody> RigidBody::getPointer()
//{
//    return shared_from_this();
//}
