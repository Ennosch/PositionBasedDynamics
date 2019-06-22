#include "include/dynamics/RigidBody.h"

RigidBody::RigidBody()
{

}

RigidBody::RigidBody(ModelPtr _model)
{
//    m_model = _model;

    // deep copy model instead of
    m_model = std::make_shared<Model>();
    m_model->clone(_model);
    m_model->setHidden(false);
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
//            for_each(m_particles.begin(), m_particles.end(), [&particle](ParticleWeakPtr pNeighbour)
            for_each(m_particles.begin(), m_particles.end(), [&particle](ParticleWeakPtr pNeighbour)
            {
                if(particle != pNeighbour.lock())
                {
                    std::shared_ptr<Particle> sharedNeighbour = pNeighbour.lock();
                    particle->m_NonCollisionParticles.push_back(sharedNeighbour);
//                    particle->m_NonCollisionParticles.push_back(pNeighbour);
                }
            });
        }
    }
    //// (?) check std::move
//    return smCstr
    return std::move(smCstr);
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
//                mlog<<"vertIdx: "<<vertIdx<<" position: "<<position;
            }
//            mlog<<" pointIdx: "<<pointIdx<<"    list: "<<pair.second;
        }
        shape->updateVertexBuffer();
    }
}

ModelPtr RigidBody::getModel()
{
    return  m_model;
}

const QMatrix4x4 RigidBody::getTransfrom()
{
    updateModelBuffers();
//    return m_transfrom.toMatrix();
    QMatrix4x4 identity;
    identity.setToIdentity();
    return  identity;
}

const QVector3D RigidBody::getTranslation()
{
    return QVector3D(0,0,0);
}

std::vector<ParticleWeakPtr> &RigidBody::getParticles()
{
    return m_particles;
}

int RigidBody::numParticles()
{
    return m_particles.size();
}

//std::shared_ptr<RigidBody> RigidBody::getPointer()
//{
//    return shared_from_this();
//}
