#include "dynamics/dynamicsWorld.h"

#include <Qdebug>

DynamicsWorld::DynamicsWorld()
    : m_hashGrid()
{
    qDebug()<<"DynaicsWorld ctor";
}

void DynamicsWorld::initialize()
{
    qDebug()<<"DynamicsWorld initialize";
}

void DynamicsWorld::update()
{
//    Collision Detection

//    Start with empty multimap
//    For each particle P
//      Determine which cell will contain P
//      Retrieve all particles in P‘s cell and the eight neighboring cells
//      Calculate distance from P to each of these and compare to d
//      Insert P into multimap

        for( ParticlePtr p : m_Particles)
        {
            int3 pCell = m_hashGrid.pointToCell(
                        p->position().x(),
                        p->position().y(),
                        p->position().z() );

            size_t pHash = m_hashGrid.hashFunction(pCell);
            p->setHash(pHash);
            bool hashExisted = m_hashGrid.insert(pHash, p);

            // get all neightbouring particles
            std::list<ParticlePtr> neighbourParticles = m_hashGrid.cellNeighbours(pCell);

//            for(ParticlePtr n : neightbours)
//            {

//            }

        }
}

/*
 *
 * from https://stackoverflow.com/questions/2047414/advantages-of-stdfor-each-over-for-loop
 *
 * for_each(a.begin(), a.end(), a_item_handler);
is better than

for(auto& item: a) {
    a_item_handler(a);
}
*/


void DynamicsWorld::addDynamicObject(pSceneOb _sceneObject)
{
    qDebug()<<"addDynamics Obecjt";
//    auto modelPtr = _sceneObject->pModel;
}

ParticlePtr DynamicsWorld::addParticle(float _x, float _y, float _z)
{
    qDebug()<<"ADDED PARTICLE_____________________";
    auto pParticle  = std::make_shared<Particle>(_x, _y, _z);
    m_Particles.push_back(pParticle);
    return pParticle;
}
