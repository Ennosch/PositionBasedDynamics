
#include "dynamics/dynamicsWorld.h"

#include "Scene.h"

#include <Qdebug>



DynamicsWorld::DynamicsWorld()
    : m_hashGrid()
{
    qDebug()<<"DynaicsWorld ctor";
    m_simulate = false;
    m_dt = 0.1;
    m_DynamicsWorldController = new DynamicsWorldController(this);
}

void DynamicsWorld::initialize()
{
    qDebug()<<"DynamicsWorld initialize ";
//    Eigen::Vector3d test(1,2,3);

//    mlog<<"helloo"<<test.x()<<test.y();

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

void DynamicsWorld::initialize(Scene *_scene)
{
    m_scene = _scene;
    initialize();
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
                // check predicted postion for particle particle collisions
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
//            p->p += c->deltaP();
            c->project();
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

//     modify velocity (16)
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

}

DynamicObjectPtr DynamicsWorld::addDynamicObjectAsParticle(pSceneOb _sceneObject)
{
    pCount++;
    QVector3D pos = _sceneObject->getPos();

    auto pDynamicObject = std::make_shared<Particle>(pos.x(), pos.y(), pos.z(), pCount);
    m_Particles.push_back(pDynamicObject);
    m_DynamicObjects.push_back(pDynamicObject);
    return pDynamicObject;
}

DynamicObjectPtr DynamicsWorld::addDynamicObjectAsRigidBody(pSceneOb _sceneObject)
{
    if(!_sceneObject->model())
        return nullptr;

    auto nRB = std::make_shared<RigidBody>(_sceneObject->model());
    ModelPtr model = _sceneObject->model();
    for(unsigned int i = 0; i < model->getNumShapes(); i++)
    {
        ShapePtr shape = model->getShape(i);
        mlog<<"has shape"<<i;
        for(auto vert : shape->getVertices())
        {
            QVector3D pos = vert.Position;
            mlog<<"pos:"<<pos;
            auto nParticle = std::make_shared<Particle>(pos.x(), pos.y(), pos.z(), 33);
            m_Particles.push_back(nParticle);
            nRB->addParticle(nParticle);
            m_scene->addSceneObjectFromParticle(nParticle);

        }
    }
    return nullptr;
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

void DynamicsWorld::checkSphereSphere(const ParticlePtr p1, const ParticlePtr p2)
{
    // Check predicted positon
    float d;
    if(m_CollisionDetect.checkSphereSphere(p1->p, p2->p, d,p1->radius(), p2->radius())){
        addParticleParticleConstraint(p1, p2);
    }
}

void DynamicsWorld::generateData()
{
    mlog<<"Gen Data";
//    auto nSpring = std::make_shared<DistanceEqualityConstraint>(m_Particles[0], m_Particles[1]);
//    nSpring->setRestLength(4.5);
//    m_Particles[0]->m_Constraints.push_back(nSpring);
//    m_Particles[1]->m_Constraints.push_back(nSpring);

//    m_Constraints.push_back(nSpring);

//    auto nSpring2 = std::make_shared<DistanceEqualityConstraint>(m_Particles[0], m_Particles[2]);
//    nSpring2->setRestLength(4.5);
//    m_Particles[0]->m_Constraints.push_back(nSpring2);
//    m_Particles[2]->m_Constraints.push_back(nSpring2);

//    m_Constraints.push_back(nSpring2);


//    m_debugLines.push_back(&m_Particles[0]->x);
//    m_debugLines.push_back(&m_Particles[1]->x);
//    m_debugLines.push_back(&m_Particles[0]->x);
//    m_debugLines.push_back(&m_Particles[2]->x);
}

QVector3D* DynamicsWorld::debugDrawLineData()
{
//    auto pp9 =  m_debugLines.data();
//    QVector3D* ptr = *pp9;
//    qDebug()<<"DEBUG: ---------------"<<typeid(pp9).name();
    return nullptr;
}

void DynamicsWorld::checkSpherePlane(const ParticlePtr p1, const Plane &_plane)
{
    float dist = m_CollisionDetect.distanceFromPointToPlane(p1->p, _plane.Normal, (_plane.Offset + (p1->r *_plane.Normal)));
    if(dist > 0)
        return;
    QVector3D qc = m_CollisionDetect.intersectRayPlane(p1->x, p1->p, _plane.Normal, (_plane.Offset + (p1->r *_plane.Normal)));
    auto hsCstr = std::make_shared<HalfSpaceConstraint>(p1, qc, _plane.Normal);
//    ConstraintWeakPtr hsCstrWeak= hsCstr;
//    m_Constraints.push_back(hsCstr);
//    p1->m_CollisionConstraints.push_back(hsCstrWeak);
    p1->m_CollisionConstraints.push_back(hsCstr);
}

void DynamicsWorld::addParticleParticleConstraint(const ParticlePtr _p1, const ParticlePtr _p2)
{
    auto ppCstr = std::make_shared<ParticleParticleConstraint>(_p1, _p2);
    _p1->m_CollisionConstraints.push_back(ppCstr);
    _p2->m_CollisionConstraints.push_back(ppCstr);

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











