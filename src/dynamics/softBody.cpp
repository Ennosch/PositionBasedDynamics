#include "include/dynamics/softBody.h"

SoftBody::SoftBody()
{

}

SoftBody::SoftBody(ModelPtr _model)
{
    // clone the model. vertices are deformed (vbo is reallocated every frame) :(
    m_model = std::make_shared<Model>();
    m_model->clone(_model);
}

void SoftBody::addParticle(const QVector3D &_localPos, const ParticleWeakPtr _particle)
{
    m_particles.push_back(_particle);
}

std::vector< std::set<int> > SoftBody::createConstraintNetwork()
{
    std::vector<int> constraintIdxs;
    std::vector< std::set<int> > constraintIdxPairs;

    ShapePtr shape = getModel()->getShape(0);
    int verticesSize = shape->getVertices().size()-1;
    /* step (1) build triangle springs
     * assumed model is Quadrangulate mesh. Taking 1 triangle per quad.
     *      |\
     *      | \
     *      |___\
     */
    for(int i=2; i < verticesSize; i += 4)
    {
        std::vector<int> triVertIdxs = {i-2, i-1, i};
        std::vector<int> triPointIdx;
        for(int vertIdx : triVertIdxs)
        {
            int pointIdx = findKbyValueElement(shape->getVertsMap(), vertIdx);
//            if(pointIdx != -1)
                triPointIdx.push_back(pointIdx);
        }
        std::set<int> pairA = {triPointIdx[0], triPointIdx[1]};
        std::set<int> pairB = {triPointIdx[1], triPointIdx[2]};
        std::set<int> pairC = {triPointIdx[2], triPointIdx[0]};
        if(!hasPair(constraintIdxPairs, pairA))
            constraintIdxPairs.push_back(pairA);
        if(!hasPair(constraintIdxPairs, pairB))
            constraintIdxPairs.push_back(pairB);
        if(!hasPair(constraintIdxPairs, pairC))
            constraintIdxPairs.push_back(pairC);
    }

    /* step (2) build Z-edge springs
     *        _______>
     *      |\
     *      | \
     *      |___\
     */
    for(int i=3; i < shape->getIndices().size()-1; i += 6)
    {
        int vertIdxA = shape->getIndices()[i];
        int vertIdxB = shape->getIndices()[i+2];

        int pointIdxA = findKbyValueElement(shape->getVertsMap(), vertIdxA);
        int pointIdxB = findKbyValueElement(shape->getVertsMap(), vertIdxB);
        std::set<int> pair = {pointIdxA, pointIdxB};
        if(!hasPair(constraintIdxPairs, pair))
            constraintIdxPairs.push_back(pair);
    }

    /* step (3) build x-edge springs
     *
     *      |\   |
     *      | \  |
     *      |___\|
     *           v
     */
    for(int i=5; i < shape->getIndices().size(); i += 6)
    {
        int vertIdxA = shape->getIndices()[i];
        int vertIdxB = shape->getIndices()[i-1];

        int pointIdxA = findKbyValueElement(shape->getVertsMap(), vertIdxA);
        int pointIdxB = findKbyValueElement(shape->getVertsMap(), vertIdxB);
        std::set<int> pair = {pointIdxA, pointIdxB};
        if(!hasPair(constraintIdxPairs, pair))
            constraintIdxPairs.push_back(pair);
    }

//    for(auto p : constraintIdxPairs)
//    {
//        qDebug()<<"----";
//        for(auto x : p )
//        {
//            qDebug()<<x;
//        }
//    }
//    mlog<<"stop";

    return constraintIdxPairs;
}

void SoftBody::updateModelBuffers()
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

ModelPtr SoftBody::getModel()
{
    return m_model;
}

const QMatrix4x4 SoftBody::getTransfrom()
{
    updateModelBuffers();
//    return m_transfrom.toMatrix();
    QMatrix4x4 identity;
    identity.setToIdentity();
    return  identity;
}

std::vector<ParticleWeakPtr>& SoftBody::getParticlels()
{
    return m_particles;
}
