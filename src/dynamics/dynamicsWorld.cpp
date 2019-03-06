#include "dynamics/dynamicsWorld.h"


#include <Qdebug>



DynamicsWorld::DynamicsWorld()
    : m_hashGrid()
{
    qDebug()<<"DynaicsWorld ctor";
    m_simulate = false;
    m_dt = 0.2;
    m_DynamicsWorldController = new DynamicsWorldController(this);
}

void DynamicsWorld::initialize()
{
    qDebug()<<"DynamicsWorld initialize";
//    Eigen::Vector3d test(1,2,3);

    m_CollisionDetect = CollisionDetection();
    Plane groundPlane;
    groundPlane.Normal = QVector3D(0, 1, 0);
    groundPlane.Offset = QVector3D(0, 0, 0);
    addPlane(groundPlane);

//    auto nSpring = std::make_shared<DistanceEqualityConstraint>(m_Particles[0], m_Particles[1]);

    for( ParticlePtr p : m_Particles)
    {
        p->pp = QVector3D(0,0,0);
    }
}

void DynamicsWorld::update()
{
    float dt = m_dt;

    if(!m_simulate)
        return;

    // PBD Loop start
    // explicit Euler integration step (5)
    for( ParticlePtr p : m_Particles)
    {
        // e.G. gravity 0, 1, 0
        QVector3D forceExt = QVector3D(0, -1, 0);
        p->v = p->v + dt * p->w*forceExt;
    }

    // damp Velocities (6)

    for( ParticlePtr p : m_Particles)
    {
        p->p = p->x + dt * p->v;
    }

    //  Collision Detection (8)
    //  spatial Hash for particle particle collision
    m_hashGrid.clear();
    for( ParticlePtr p : m_Particles)
    {
        //    Start with empty multimap
        //    For each particle P
        //      Determine which cell will contain P
        //      Retrieve all particles in P‘s cell and the eight neighboring cells
        //      Calculate distance from P to each of these and compare to d
        //      Insert P into multimap
//            qDebug()<<"p: "<<p->ID<<"x: "<<p->position().x()<<"y: "<<p->position().y()<<"z: "<<p->position().z();
        int3 pCell = m_hashGrid.pointToCell(
                    p->position().x(),
                    p->position().y(),
                    p->position().z() );

//            qDebug()<<"Position: "<<p->position().x()<<p->position().y()<<p->position().z();
//            qDebug()<<"Cell: "<<pCell.i<<pCell.j<<pCell.k;

        size_t pHash = m_hashGrid.hashFunction(pCell);
        p->setHash(pHash);
        bool hashExisted = m_hashGrid.insert(pHash, p);
        auto test = m_hashGrid.query(pHash);
        // get all neightbouring particles
        std::list<ParticlePtr> neighbourParticles = m_hashGrid.cellNeighbours(pCell);
        for(ParticlePtr n : neighbourParticles)
        {
            if(n != p)
                checkSphereSphere(p,n);
        }
        checkSpherePlane(p, m_Planes[0]);

    }

    // Solver Iteration (9)
    for( ParticlePtr p : m_Particles)
    {
        for( ConstraintWeakPtr c : p->m_Constraints)
        {
//            p->p += c->deltaP();
//            auto test = c.lock();
//            bool test2 = c.expired();
            if(auto constraint = c.lock())
                constraint->project();
        }

        for( ConstraintPtr c : p->m_CollisionConstraints)
        {
//            if(auto constraint = c.lock())
//                p->p += constraint->deltaP();
            p->p += c->deltaP();
        }
        p->m_CollisionConstraints.clear();
    }


    // Apply correction (13,14)
    for( ParticlePtr p : m_Particles)
    {
        p->v = (p->p - p->x) / dt;
        p->x = p->p;
//        p->x = p->x + QVector3D(0, -0.01, 0);
    }

    // modify velocity (16)
//    for( ParticlePtr p : m_Particles)
//    {
//        p->v = p->v + p->pp;
//        p->pp = QVector3D(0,0,0);
//    }

}

void DynamicsWorld::info()
{
    qDebug()<<"p1: "<<m_Particles[0]->x<<m_Particles[0]->ID<<m_Particles[0]->r;
}

DynamicsWorldController* DynamicsWorld::controller()
{
    if(m_DynamicsWorldController)
        return m_DynamicsWorldController;
    return nullptr;
}

void DynamicsWorld::setSimulate(bool _isSimulating)
{
    m_simulate = _isSimulating;
}

void DynamicsWorld::step()
{
    m_simulate = true;
    update();
    m_simulate = false;
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

DynamicObjectPtr DynamicsWorld::addDynamicObjectAsParticle(pSceneOb _sceneObject, ParticlePtr _particle)
{
    qDebug()<<"addDynamicObjectAsParticle ";
    pCount++;
    auto pDynamicObject = std::make_shared<DynamicObject>(_particle);
    m_DynamicObjects.push_back(pDynamicObject);
    return pDynamicObject;
}

ParticlePtr DynamicsWorld::addParticle(float _x, float _y, float _z)
{
    qDebug()<<"ADDED PARTICLE_____________________";
    pCount++;
    auto pParticle  = std::make_shared<Particle>(_x, _y, _z, pCount);
    m_Particles.push_back(pParticle);
    return pParticle;
}

void DynamicsWorld::addPlane(const Plane &_plane)
{
//    auto pDynamicObject = std::make_shared<DynamicObject>();
    m_Planes.push_back(_plane);
}

void DynamicsWorld::checkSphereSphere(ParticlePtr p1, ParticlePtr p2)
{

}

void DynamicsWorld::generateData()
{

//    auto pinCnstr = std::make_shared<PinConstraint>(m_Particles[0], QVector3D(1,1,0));

//    m_Particles[0]->m_Constraints.push_back(pinCnstr);
    qDebug()<<"genData";

    auto nSpring = std::make_shared<DistanceEqualityConstraint>(m_Particles[0], m_Particles[1]);
    nSpring->setRestLength(6.5);
    m_Particles[0]->m_Constraints.push_back(nSpring);
    m_Particles[1]->m_Constraints.push_back(nSpring);

    m_Constraints.push_back(nSpring);

//    bool test1 = m_Particles[0]->m_Constraints[0].expired();
//    auto test = m_Particles[0]->m_Constraints[0].lock();

    auto nSpring2 = std::make_shared<DistanceEqualityConstraint>(m_Particles[0], m_Particles[2]);
    nSpring2->setRestLength(4.5);
    m_Particles[0]->m_Constraints.push_back(nSpring2);
    m_Particles[2]->m_Constraints.push_back(nSpring2);


    m_Constraints.push_back(nSpring2);

//    m_debugLines.push_back(&test1);
    m_debugLines.push_back(&m_Particles[0]->x);
    m_debugLines.push_back(&m_Particles[1]->x);
    m_debugLines.push_back(&m_Particles[0]->x);
    m_debugLines.push_back(&m_Particles[2]->x);
}

QVector3D* DynamicsWorld::debugDrawLineData()
{
//    auto pp9 =  m_debugLines.data();
//    QVector3D* ptr = *pp9;
//    qDebug()<<"DEBUG: ---------------"<<typeid(pp9).name();
    return nullptr;
}

void DynamicsWorld::checkSpherePlane(ParticlePtr p1, const Plane &_plane)
{
    float dist = m_CollisionDetect.checkPointPlane(p1->p, _plane.Normal, (_plane.Offset + (p1->r *_plane.Normal)));
    if(dist > 0)
        return;

    QVector3D qc = m_CollisionDetect.checkRayPlane(p1->x, p1->p, _plane.Normal, (_plane.Offset + (p1->r *_plane.Normal)));

//    float t, devisor;
//    l = (p1->p - p1->x).normalized();
//    devisor = QVector3D::dotProduct(l, _plane.Normal);
//    t = (QVector3D::dotProduct((o-x), n)) / devisor;
//    auto hsCstr = std::make_shared<ConstraintPtr>

    auto hsCstr = std::make_shared<HalfSpaceConstraint>(p1->p, qc, _plane.Normal);
//    ConstraintWeakPtr hsCstrWeak= hsCstr;
//    m_Constraints.push_back(hsCstr);
//    p1->m_CollisionConstraints.push_back(hsCstrWeak);
    p1->m_CollisionConstraints.push_back(hsCstr);
}

void DynamicsWorld::deleteConstraint(const ConstraintPtr _constraint)
{
    for(auto p : _constraint->m_Particles)
    {
        if(auto particle = p.lock())
        {
            std::vector< ConstraintWeakPtr>::const_iterator it = particle->m_Constraints.begin();
            std::vector< ConstraintWeakPtr>::const_iterator itEnd = particle->m_Constraints.end();
            for(it; it != itEnd; ++it)
            {
                if(it->lock() == _constraint)
                {
                    particle->m_Constraints.erase(it);
                }
            }
        }
    }

    m_Constraints.erase(
                std::remove_if(
                    m_Constraints.begin(),
                    m_Constraints.end(),
                    [&](const ConstraintPtr c){return c == _constraint;}),
                m_Constraints.end()
                );

}




/*
 *
 *
 *
 *
 *
 */











